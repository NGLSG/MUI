#ifndef UIMANAGER_H
#define UIMANAGER_H
#include "Event.h"
#include "UIBase.h"

namespace Mio {
    class UIManager : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIManager>>;

        UIManager() {
            IsManager = true;
            AddComponent<Event>();
        }

        std::shared_ptr<UIBase> GetUIElement(UUid uuid);

        void AddUIElement(const std::shared_ptr<UIBase>&uiElement);

        void AddUIElements(const std::vector<std::shared_ptr<UIBase>>&uiElements);

        void RemoveUIElement(UUid uuid);

        void RemoveUIElement(const std::string&name);

        void Update() override;

        void AddCustomGUIRendering(const std::string&name);

        void RemoveCustomGUIRendering(const std::string&name);

        std::vector<std::shared_ptr<UIBase>> uiElements;
        std::set<std::shared_ptr<UIBase>> uniqueUIElements;
    };
}


#endif //UIMANAGER_H
