#include "UIBase.h"
#include <imgui.h>

namespace Mio {
    void UIBase::BeginFrame() {
        ImGui::PushID(uuid.toString().c_str());
        style.BeginFrame();
        transform.BeginFrame();
        if (!NoChild)
            ImGui::BeginChild(uuid.toString().c_str(), transform.cRetSize, true, ImGuiWindowFlags_NoScrollbar);
    }

    void UIBase::EndFrame() {
        if (!NoChild)
            ImGui::EndChild();
        transform.EndFrame();
        style.EndFrame();
        ImGui::PopID();
    }
} // Mio
