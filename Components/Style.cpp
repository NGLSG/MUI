#include "Style.h"


namespace Mio {
    void Style::BeginFrame() {
        copy = ImGui::GetStyle();
        ImGui::GetStyle() = cStyle;
    }


    void Style::EndFrame() {
        ImGui::GetStyle() = copy;
    }
} // Mio
