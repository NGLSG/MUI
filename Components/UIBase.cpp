#include "UIBase.h"
#include <imgui.h>

namespace Mio {
    void UIBase::BeginFrame() {
        ImGui::PushID(uuid.toString().c_str());


        style.BeginFrame();
        if (!IsManager) {
            transform.BeginFrame();
            ImGui::BeginChild(uuid.toString().c_str(), transform.cRetSize, true, ImGuiWindowFlags_NoScrollbar);
        }
    }

    void UIBase::EndFrame() {
        if (!IsManager) {
            ImGui::EndChild();
            transform.EndFrame();
        }

        style.EndFrame();
        ImGui::PopID();
    }
} // Mio
