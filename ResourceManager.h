#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <fstream>
#include <future>
#include <thread>
#include <utility>

#include "Application.h"
#include "GUIManifest.h"

namespace Mio {
    struct Manifests;

    struct ResourceIndex {
        std::string name;
        size_t offset;
        size_t size;
    };

    class ResourcePackage {
    public:
        static std::shared_ptr<ResourcePackage> Load(const std::string&path);

        static void AddResourceToPackage(const std::string&resourcePath, std::ofstream&packageFile,
                                         std::vector<ResourceIndex>&indexTable) {
            std::ifstream resourceStream(resourcePath, std::ios::binary);
            if (!resourceStream) {
                std::cerr << "Error: Cannot open resource file." << std::endl;
                return;
            }

            // 读取整个资源文件到缓冲区
            std::vector<char> resourceData(std::istreambuf_iterator<char>(resourceStream), {});
            resourceStream.close();

            // 写入资源数据到打包文件
            packageFile.write(resourceData.data(), resourceData.size());

            // 记录索引信息
            ResourceIndex indexEntry;
            indexEntry.name = resourcePath;
            indexEntry.offset = packageFile.tellp(); // 资源数据的起始偏移量
            indexEntry.size = resourceData.size();
            indexTable.push_back(indexEntry);
        }

    private:
    };

    class ResourceManager {
    public:
        static std::shared_ptr<GUIManifest> LoadManifest(const std::string&name);

        static void LoadManifest(const std::string&name, std::shared_ptr<GUIManifest> manifest);

        static bool SaveManifest(const std::shared_ptr<GUIManifest>&manifest);

        static Manifests LoadManifests(std::string path);

        template<typename T>
        static std::shared_ptr<T> GetUIByName(const std::shared_ptr<GUIManifest>&manifest, const std::string&name) {
            if (UIResources.contains(manifest))
                return std::dynamic_pointer_cast<T>(UIResources[manifest][name]);
            return nullptr;
        }

        template<typename T>
        static std::shared_ptr<T> GetUIByUID(const std::shared_ptr<GUIManifest>&manifest, const UUid&uid) {
            if (UIResources.contains(manifest)) {
                for (auto&ui: UIResources[manifest] | std::views::values) {
                    if (ui->UID() == uid)
                        return std::dynamic_pointer_cast<T>(ui);
                }
            }
            return nullptr;
        }

        template<typename T>
        static bool Save(const T&data, const std::string&path) {
            std::ofstream file(path);
            if (!file.is_open()) {
                return false;
            }
            file << YAML::convert<T>::encode(data);
            file.close();
            return true;
        }

        template<typename T>
        static T Load(const std::string&path) {
            std::ifstream file(path);
            if (!file.is_open()) {
                throw std::runtime_error("Unable to open file for loading");
            }
            YAML::Node node = YAML::Load(file);
            file.close();
            T data;
            YAML::convert<T>::decode(node, data);
            return data;
        }

        inline static std::unordered_map<std::string, std::shared_ptr<GUIManifest>> Manifests = {};

    private:
        inline static std::unordered_map<std::shared_ptr<GUIManifest>, std::unordered_map<std::string, std::shared_ptr<
            UIBase>>>
        UIResources = {};
    };

    struct Manifests {
        enum class LoadState {
            Loading,
            Loaded,
            Failed
        };

        std::map<std::string, std::string> ManifestsList;

        std::shared_ptr<GUIManifest> Load(const std::string&name) {
            return std::move(ResourceManager::LoadManifest(ManifestsList[name]));
        }

        void Load(const std::string&name, std::shared_ptr<GUIManifest> manifest) {
            ResourceManager::LoadManifest(name, std::move(manifest));
        }

        LoadState& LoadAysnc(const std::string&name, std::shared_ptr<GUIManifest> manifest) {
            LoadStates[name] = LoadState::Loading;
            std::thread([this, name, manifest]() {
                Load(name, manifest);
                LoadStates[name] = LoadState::Loaded;
            }).detach();
            return LoadStates[name];
        }

        std::future<std::shared_ptr<GUIManifest>> LoadAsysnc(const std::string&name) {
            return std::async([this, name]() {
                return Load(name);
            });
        }

        LoadState& LoadAsync(const std::string&name) {
            LoadStates[name] = LoadState::Loading;
            std::thread([this, name]() {
                Load(name);
                LoadStates[name] = LoadState::Loaded;
            }).detach();
            return LoadStates[name];
        }

        LoadState GetLoadState(const std::string&name) {
            return LoadStates[name];
        }

        std::shared_ptr<GUIManifest> Get(const std::string&name) {
            auto res = std::move(Results[name]);
            Results.erase(name);
            return res;
        }

    private:
        std::map<std::string, LoadState> LoadStates;
        std::map<std::string, std::shared_ptr<GUIManifest>> Results;
    };
}

namespace YAML {
    template<>
    struct convert<Mio::Manifests> {
        static Node encode(const Mio::Manifests&manifests) {
            Node node;
            for (auto&[name, path]: manifests.ManifestsList) {
                node[name] = path;
            }
            return node;
        }

        static bool decode(const Node&node, Mio::Manifests&manifests) {
            for (auto&[name, path]: node.as<std::map<std::string, std::string>>()) {
                manifests.ManifestsList[name] = path;
            }
            return true;
        }
    };
}


#endif //RESOURCEMANAGER_H
