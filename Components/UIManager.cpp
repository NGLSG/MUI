#include "UIManager.h"

namespace Mio {
    std::shared_ptr<UIBase> UIManager::GetUIElement(UUid uuid) {
        auto it = std::find_if(uiElements.begin(), uiElements.end(),
                               [uuid](const std::shared_ptr<UIBase>&item) {
                                   return item->UID() == uuid;
                               });


        if (it != uiElements.end()) {
            return *it;
        }

        return nullptr;
    }

    void UIManager::AddUIElement(const std::shared_ptr<UIBase>&uiElement) {
        if (uniqueUIElements.insert(uiElement).second)
            uiElements.emplace_back(uiElement);
    }

    void UIManager::AddUIElements(const std::vector<std::shared_ptr<UIBase>>&uiElements) {
        for (const auto&uiElement: uiElements) {
            AddUIElement(uiElement);
        }
    }

    void UIManager::RemoveUIElement(UUid uuid) {
        std::erase_if(uiElements, [&](const std::shared_ptr<UIBase>&item) {
            return item->UID() == uuid;
        });
    }

    void UIManager::RemoveUIElement(const std::string&name) {
        std::erase_if(uiElements, [&](const std::shared_ptr<UIBase>&item) {
            return item->cName == name;
        });
    }

    void UIManager::Update() {
        GetComponent<Event>().BeginFrame();
        for (auto&uiElement: uiElements) {
            uiElement->Update();
        }
    }

    void UIManager::AddCustomGUIRendering(const std::string&name) {
        GetComponent<Event>().AddListener(name);
    }

    void UIManager::RemoveCustomGUIRendering(const std::string&name) {
        GetComponent<Event>().RemoveListener(name);
    }
}
