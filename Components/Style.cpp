#include "Style.h"


namespace Mio {
    void Style::BeginFrame() {
        auto&style = ImGui::GetStyle();
        style = cStyle;
    }


    void Style::EndFrame() {
        auto&style = ImGui::GetStyle();
        style = DefaultStyle;
    }
} // Mio
