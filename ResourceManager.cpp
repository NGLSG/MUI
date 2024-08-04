#include "ResourceManager.h"

namespace Mio {
    std::shared_ptr<GUIManifest> ResourceManager::LoadManifest(const std::string&name) {
        auto manifest = std::make_shared<GUIManifest>(Load<GUIManifest>(name + ".manifest"));
        for (auto&manager: manifest->sManager) {
            UIResources[manifest].emplace(manager->cName, manager);
            for (auto&ui: manager->uiElements) {
                UIResources[manifest].emplace(ui->cName, ui);
            }
        }
        Manifests[name] = manifest;
        return manifest;
    }

    bool ResourceManager::SaveManifest(const std::shared_ptr<GUIManifest>&manifest) {
        return Save<GUIManifest>(*manifest, manifest->sName + ".manifest");
    }
}
