//
// Created by 92703 on 2024/7/30.
//

#include "Transform.h"

#include <imgui_internal.h>

Mio::Transform::Transform() {
}

Mio::Transform::Transform(const Transform&other) {
    this->cPosition = other.cPosition;
    this->cSize = other.cSize;
    this->cScale = other.cScale;
}

Mio::Transform& Mio::Transform::operator=(const Transform&other) {
    this->cPosition = other.cPosition;
    this->cSize = other.cSize;
    this->cScale = other.cScale;
    return *this;
}


void Mio::Transform::BeginFrame() {
    ImVec2 originalPos = ImGui::GetWindowPos();
    if (cPosition.x != 0 || cPosition.y != 0)
        originalPos += cPosition;

    ImGui::SetCursorScreenPos(originalPos);
    cRetSize = {cSize.x * cScale.x, cScale.y * cSize.y};
    ImGui::SetNextWindowSize(cRetSize);
}

void Mio::Transform::EndFrame() {
}


float Mio::Transform::Radians(float degrees) {
    return degrees * (3.14159265358979323846f / 180);
}
