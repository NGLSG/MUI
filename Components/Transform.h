#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <imgui.h>

#include "Component.h"


namespace Mio {
    class Transform : Component {
    public:
        Transform();

        Transform(const Transform&other);

        Transform& operator=(const Transform&other);

        void SetTransform(Transform other) {
            cScale = other.cScale;
            cPosition = other.cPosition;
            cSize = other.cSize;
        }

        void BeginFrame() override;

        void EndFrame() override;

        ImVec2 cScale = {1, 1};
        ImVec2 cPosition = {0, 0};
        ImVec2 cSize = {32, 32};
        ImVec2 cRetSize;

    private:
        static float Radians(float degrees);
    };
}


#endif //TRANSFORM_H
