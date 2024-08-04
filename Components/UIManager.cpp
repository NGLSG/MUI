#include "UIManager.h"

namespace Mio {
    std::shared_ptr<UIBase> UIManager::GetUiElement(UUid uuid) {
        auto it = std::find_if(uiElements.begin(), uiElements.end(),
                               [uuid](const std::shared_ptr<UIBase>&item) {
                                   return item->UID() == uuid;
                               });


        if (it != uiElements.end()) {
            return *it;
        }

        return nullptr;
    }

    void UIManager::AddUiElement(const std::shared_ptr<UIBase>&uiElement) {
        uiElements.emplace_back(uiElement);
    }

    void UIManager::RemoveUiElement(UUid uuid) {
        std::erase_if(uiElements, [&](const std::shared_ptr<UIBase>&item) {
            return item->UID() == uuid;
        });
    }

    void UIManager::BeginFrame() {
        for (auto&uiElement: uiElements) {
            uiElement->Update();
        }
    }
}
