#ifndef UIBASE_H
#define UIBASE_H
#include <yaml-cpp/node/convert.h>

#include "Component.h"

#include "Style.h"
#include "Transform.h"

namespace Mio {
    class UIBase : public Component {
    public:
        friend class YAML::convert<UIBase>;
        friend class YAML::convert<std::shared_ptr<UIBase>>;

        enum class Type {
            Button,
            CheckBox,
            ColorEditor,
            ColorPicker,
            Combo,
            Image,
            ImageButton,
            InputText,
            ListBox,
            MenuBar,
            MenuItem,
            ProgressBar,
            RadioButton,
            Slider,
            TreeNode,
            SubWindow,
            Text,
            Tooltip,
            Window,
            Group,
            Popup,
            Console,
            None
        };

        struct Data {
        };

        UIBase() {
        }

        Transform&transform = AddComponent<Transform>();
        Style&style = AddComponent<Style>();

        void SetActive(bool active) {
            Active = active;
        }

        bool ActiveSelf() const {
            return Active;
        }

        Type cType;
        Data cData;
        std::string cName;

    protected:
        void BeginFrame() override;

        void EndFrame() override;

        std::vector<std::string> RefVariables;


        bool Active = true;
        bool NoChild = false;
    };
} // Mio

#endif //UIBASE_H
