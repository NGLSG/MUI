#ifndef LAYOUT_H
#define LAYOUT_H
#include "Component.h"
#include "Transform.h"

namespace Mio {
    struct Horizontal {
        float Left = 0;
        float Right = 0;

        Horizontal() = default;
    };

    struct Vertical {
        float Up = 0;
        float Down = 0;

        Vertical() = default;
    };

    class Layout : public Component {
    public:
        enum Type {
            Left = 0x01,
            Right = 0x02,
            Top = 0x04,
            Bottom = 0x08,
            Center = 0x10,
            Stretch = 0x20,
            Fill = Left | Right | Top | Bottom,
        };

        Layout(Type layoutType, Transform&transform);

        void BeginFrame() override;

        void EndFrame() override;

        ImVec2 Pos;
        Horizontal Horizontal;
        Vertical Vertical;

    private:
        bool HasType(Type type) const {
            return (cLayoutType & type) != 0;
        }

        Transform&cTransform;
        Type cLayoutType = Type::Center;
    };
} // Mio

#endif //LAYOUT_H
