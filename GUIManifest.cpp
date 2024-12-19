#include "GUIManifest.h"
#include "ResourceManager.h"

namespace Mio {
    void GUIManifest::Save() {
        ResourceManager::SaveManifest(shared_from_this());
    }

    void GUIManifest::RemoveUI(const std::string&name) {
        ResourceManager::RemoveUI(shared_from_this(), name);
        for (auto&it: sManager) {
            it->RemoveUIElement(name);
        }
    }

    void GUIManifest::RemoveUI(const UUid&uid) {
        ResourceManager::RemoveUI(shared_from_this(), uid);
        for (auto&it: sManager) {
            it->RemoveUIElement(uid);
        }
    }

    void GUIManifest::TryRemoveManager(const std::string&name) {
        for (auto it = sManager.begin(); it != sManager.end(); ++it) {
            if ((*it)->cName == name) {
                sManager.erase(it);
                return;
            }
        }
    }

    void GUIManifest::TryAddManager(const std::shared_ptr<UIManager>&manager) {
        if (std::find(sManager.begin(), sManager.end(), manager) == sManager.end()) {
            sManager.push_back(manager);
        }
    }

    GUIManifest::GUIManifest() = default;

    GUIManifest::GUIManifest(const std::string&name, const std::string&dir) {
        sName = name;
        SavePath = dir + "/" + sName;
    }
}
