#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <fstream>

#include "UI.h"
#include "GUIManifest.h"

namespace Mio {
    class ResourceManager {
    public:
        static std::shared_ptr<GUIManifest> LoadManifest(const std::string&name);

        static bool SaveManifest(const std::shared_ptr<GUIManifest>&manifest);

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
}


#endif //RESOURCEMANAGER_H
