#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <fstream>
#include <future>
#include <thread>
#include <utility>

#include "Application.h"
#include <filesystem>
#include "Compression.h"
#include <yaml-cpp/yaml.h>

#include "Utils/Utils.h"

inline static const std::string ResourcePackageExtension = ".pck";
inline static const std::filesystem::path ResourcePath("Resources");
inline static const std::filesystem::path PackedPath("build");
inline static const char* PASSWORD = "Cj5WX_nyC7mhWL[CjdPL.DnqDvXsgcbB";
inline static const char* IV = ")1RUg5}mL$~62#Lw";
inline static constexpr RC::Type ComressionType = RC::Type::TAR_BZ2;

namespace Mio {
    class GUIManifest;

    class ResourcePackage : public std::enable_shared_from_this<ResourcePackage> {
    public:
        ResourcePackage(std::string name, std::vector<std::string> files = {}): name(std::move(name)),
            files(std::move(files)) {
        }

        static std::shared_ptr<ResourcePackage> Create(std::string name, std::vector<std::string> files = {}) {
            return std::make_shared<ResourcePackage>(std::move(name), files);
        }

        void AddFiles(std::vector<std::string> files);

        std::string AddFile(std::string file);

        bool Pack();

        bool UnpackAll();

        std::string Unpack(std::string selectFile);

    private:
        std::string name;
        std::vector<std::string> files;
        std::string finalpath = (PackedPath / name).string() + ResourcePackageExtension;
        std::string path = (PackedPath / name).string() + ".tmp";
    };

    class ResourcePackageManager {
    public:
        static std::shared_ptr<ResourcePackage> Load(const std::string&path);

        static void AddResource(std::shared_ptr<ResourcePackage> package, std::vector<std::string> files);

        static bool Pack(std::shared_ptr<ResourcePackage> package);

        static bool UnpackAll(std::shared_ptr<ResourcePackage> package);

        static std::string Unpack(std::shared_ptr<ResourcePackage> package, std::string selectFile);
    };

    class ResourceManager {
    public:
        static std::shared_ptr<GUIManifest> LoadManifest(const std::string&name);

        static void LoadManifest(const std::string&name, std::shared_ptr<GUIManifest> manifest);

        static bool SaveManifest(const std::shared_ptr<GUIManifest>&manifest);

        template<typename T>
        static std::shared_ptr<T> GetUI(const std::shared_ptr<GUIManifest>&manifest, const std::string&name) {
            if (contains(UIResources, manifest))
                return std::dynamic_pointer_cast<T>(UIResources[manifest][name]);
            return nullptr;
        }

        static void RemoveUI(const std::shared_ptr<GUIManifest>&manifest, const std::string&name) {
            if (contains(UIResources, manifest))
                UIResources[manifest].erase(name);
        }

        template<typename T>
        static std::shared_ptr<T> GetUI(const std::shared_ptr<GUIManifest>&manifest, const UUid&uid) {
            if (contains(UIResources, manifest)) {
                for (auto&pair: UIResources[manifest]) {
                    auto&ui = pair.second;
                    if (ui->UID() == uid)
                        return std::dynamic_pointer_cast<T>(ui);
                }
            }
            return nullptr;
        }

        static void RemoveUI(const std::shared_ptr<GUIManifest>&manifest, const UUid&uid) {
            if (contains(UIResources, manifest)) {
                for (auto it = UIResources[manifest].begin(); it != UIResources[manifest].end();) {
                    auto&ui = it->second;
                    if (ui->UID() == uid) {
                        it = UIResources[manifest].erase(it);
                        return;
                    }
                    else {
                        ++it;
                    }
                }
            }
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

    inline static ResourcePackage Resource("Resource", {ResourcePath.string()});
}

#define RESOURCING(x) Mio::Resource.AddFile(x)

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
