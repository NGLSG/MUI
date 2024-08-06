#include "ResourceManager.h"

namespace Mio {
    std::shared_ptr<GUIManifest> ResourceManager::LoadManifest(const std::string&name) {
        auto manifest = std::make_shared<GUIManifest>(Load<GUIManifest>(name + ".manifest"));
        for (auto&manager: manifest->sManager) {
            UIResources[manifest][manager->cName] = manager;
            for (auto&ui: manager->uiElements) {
                UIResources[manifest][ui->cName] = ui;
            }
        }
        Manifests[name] = manifest;
        return std::move(manifest);
    }

    void ResourceManager::LoadManifest(const std::string&name, std::shared_ptr<GUIManifest> manifest) {
        auto tmp = Load<GUIManifest>(manifest->SavePath + ".manifest");
        manifest->sName = tmp.sName;
        manifest->sManager = tmp.sManager;
        for (auto&manager: tmp.sManager) {
            UIResources[manifest][tmp.sName] = manager;
            for (auto&ui: manager->uiElements) {
                UIResources[manifest][ui->cName] = ui;
            }
        }
        Manifests[name] = manifest;
    }

    bool ResourceManager::SaveManifest(const std::shared_ptr<GUIManifest>&manifest) {
        return Save<GUIManifest>(*manifest, manifest->SavePath + ".manifest");
    }

    Manifests ResourceManager::LoadManifests(std::string path) {
        return Load<Mio::Manifests>(path);
    }
}
