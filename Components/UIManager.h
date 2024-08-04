#ifndef UIMANAGER_H
#define UIMANAGER_H
#include "UIBase.h"

namespace Mio {
    class UIManager : public UIBase {
    public:
        friend class YAML::convert<std::shared_ptr<UIManager>>;

        std::shared_ptr<UIBase> GetUiElement(UUid uuid);

        void AddUiElement(const std::shared_ptr<UIBase>&uiElement);

        void RemoveUiElement(UUid uuid);

        void BeginFrame() override;

        std::vector<std::shared_ptr<UIBase>> uiElements;
    };
}


#endif //UIMANAGER_H
