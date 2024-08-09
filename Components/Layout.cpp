#include "Layout.h"
//TODO
namespace Mio {
    Layout::Layout(Type layoutType, Transform&transform): cTransform(transform), cLayoutType(layoutType) {
    }

    void Layout::BeginFrame() {
        Component::BeginFrame();
        auto&size = cTransform.cSize;
        auto&pos = cTransform.cPosition;
        auto windowSize = ImGui::GetWindowSize();
        auto windowPos = ImGui::GetWindowPos();
        ImVec2 origin = {0, 0};
        if (HasType(Stretch)) {
            if (HasType(Top)) {
                origin = {windowPos.x + windowSize.x / 2, windowPos.y};
            }
        }
        else if (HasType(Fill)) {
            size = windowSize;
        }
    }

    void Layout::EndFrame() {
        Component::EndFrame();
    }
} // Mio
