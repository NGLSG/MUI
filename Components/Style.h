#ifndef STYLE_H
#define STYLE_H
#include "Component.h"
#include <imgui.h>
#include <variant>
#include <cstddef>

#define EPSILON 0.0001f

namespace Mio {
    inline bool operator==(const ImVec2 lhs, const ImVec2&rhs) {
        return std::abs(lhs.x - rhs.x) < EPSILON && std::abs(lhs.y - rhs.y) < EPSILON;
    }

    inline bool operator==(const ImVec4 lhs, const ImVec4&rhs) {
        return std::abs(lhs.x - rhs.x) < EPSILON && std::abs(lhs.y - rhs.y) < EPSILON &&
               std::abs(lhs.z - rhs.z) < EPSILON && std::abs(lhs.w - rhs.w) < EPSILON;
    }

    struct ChangeLogs {
        std::map<std::string, float> LogFloat = {};
        std::map<std::string, ImVec2> LogVec2 = {};
        std::map<std::string, ImVec4[ImGuiCol_COUNT]> LogVec4 = {};
        std::map<int, ImVec4> LogColor = {};
        std::map<std::string, ImGuiDir> LogDir = {};
        std::map<std::string, ImGuiHoveredFlags> LogHoveredFlags = {};
        std::map<std::string, bool> LogBool = {};

        inline static const std::vector<std::pair<std::string, std::string>> AllVariables = {
            {"Alpha", "float"}, {"DisabledAlpha", "float"}, {"WindowPadding", "ImVec2"},
            {"WindowRounding", "float"}, {"WindowBorderSize", "float"}, {"WindowMinSize", "ImVec2"},
            {"WindowTitleAlign", "ImVec2"}, {"WindowMenuButtonPosition", "ImGuiDir"},
            {"ChildRounding", "float"}, {"ChildBorderSize", "float"}, {"PopupRounding", "float"},
            {"PopupBorderSize", "float"}, {"FramePadding", "ImVec2"}, {"FrameRounding", "float"},
            {"FrameBorderSize", "float"}, {"ItemSpacing", "ImVec2"}, {"ItemInnerSpacing", "ImVec2"},
            {"CellPadding", "ImVec2"}, {"TouchExtraPadding", "ImVec2"}, {"IndentSpacing", "float"},
            {"ColumnsMinSpacing", "float"}, {"ScrollbarSize", "float"}, {"ScrollbarRounding", "float"},
            {"GrabMinSize", "float"}, {"GrabRounding", "float"}, {"LogSliderDeadzone", "float"},
            {"TabRounding", "float"}, {"TabBorderSize", "float"}, {"TabMinWidthForCloseButton", "float"},
            {"TabBarBorderSize", "float"}, {"TabBarOverlineSize", "float"}, {"TableAngledHeadersAngle", "float"},
            {"TableAngledHeadersTextAlign", "ImVec2"}, {"ColorButtonPosition", "ImGuiDir"},
            {"ButtonTextAlign", "ImVec2"}, {"SelectableTextAlign", "ImVec2"}, {"SeparatorTextBorderSize", "float"},
            {"SeparatorTextAlign", "ImVec2"}, {"SeparatorTextPadding", "ImVec2"}, {"DisplayWindowPadding", "ImVec2"},
            {"DisplaySafeAreaPadding", "ImVec2"}, {"DockingSeparatorSize", "float"}, {"MouseCursorScale", "float"},
            {"AntiAliasedLines", "bool"}, {"AntiAliasedLinesUseTex", "bool"}, {"AntiAliasedFill", "bool"},
            {"CurveTessellationTol", "float"}, {"CircleTessellationMaxError", "float"},
            {"Colors", "ImVec4[ImGuiCol_COUNT]"}, {"HoverStationaryDelay", "float"},
            {"HoverDelayShort", "float"}, {"HoverDelayNormal", "float"},
            {"HoverFlagsForTooltipMouse", "ImGuiHoveredFlags"}, {"HoverFlagsForTooltipNav", "ImGuiHoveredFlags"}
        };

        inline static std::vector<std::string> GetVariablesOfType(const std::string&type) {
            std::vector<std::string> result;
            for (const auto&var: AllVariables) {
                if (var.second == type) {
                    result.push_back(var.first);
                }
            }
            return result;
        }

        inline static const std::vector<std::string> FloatVariables = GetVariablesOfType("float");
        inline static const std::vector<std::string> Vec2Variables = GetVariablesOfType("ImVec2");
        inline static const std::vector<std::string> Vec4Variables = GetVariablesOfType("ImVec4[ImGuiCol_COUNT]");
        inline static const std::vector<std::string> DirVariables = GetVariablesOfType("ImGuiDir");
        inline static const std::vector<std::string> BoolVariables = GetVariablesOfType("bool");
        inline static const std::vector<std::string> HoveredFlagsVariables = GetVariablesOfType("ImGuiHoveredFlags");
    };

    class Style final : public Component {
    public:
        explicit Style(const ImGuiStyle&style) : cStyle(style) {
        }

        explicit Style() {
            cStyle = DefaultStyle;
        }

        void Modify(const ImGuiStyle&data) {
            cStyle = data;
            DiffFrom(DefaultStyle);
        }

        void Modify(const ChangeLogs&data) {
            for (const auto&[key, value]: data.LogFloat) {
                auto it = std::find_if(ChangeLogs::AllVariables.begin(), ChangeLogs::AllVariables.end(),
                                       [&key](const std::pair<std::string, std::string>&p) {
                                           return p.first == key;
                                       });
                if (it != ChangeLogs::AllVariables.end()) {
                    size_t index = std::distance(ChangeLogs::AllVariables.begin(), it);
                    size_t offset = offsetof(ImGuiStyle, Alpha) + index * sizeof(float);
                    *reinterpret_cast<float *>(reinterpret_cast<char *>(&cStyle) + offset) = value;
                }
            }

            for (const auto&[key, value]: data.LogVec2) {
                auto it = std::find_if(ChangeLogs::AllVariables.begin(), ChangeLogs::AllVariables.end(),
                                       [&key](const std::pair<std::string, std::string>&p) {
                                           return p.first == key;
                                       });
                if (it != ChangeLogs::AllVariables.end()) {
                    size_t index = std::distance(ChangeLogs::AllVariables.begin(), it);
                    size_t offset = offsetof(ImGuiStyle, WindowPadding) + index * sizeof(ImVec2);
                    *reinterpret_cast<ImVec2 *>(reinterpret_cast<char *>(&cStyle) + offset) = value;
                }
            }

            for (const auto&[key, value]: data.LogVec4) {
                if (key == "Colors") {
                    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
                        cStyle.Colors[i] = value[i];
                    }
                }
            }

            for (const auto&[key, value]: data.LogColor) {
                cStyle.Colors[key] = value;
            }

            for (const auto&[key, value]: data.LogDir) {
                auto it = std::find_if(ChangeLogs::AllVariables.begin(), ChangeLogs::AllVariables.end(),
                                       [&key](const std::pair<std::string, std::string>&p) {
                                           return p.first == key;
                                       });
                if (it != ChangeLogs::AllVariables.end()) {
                    size_t index = std::distance(ChangeLogs::AllVariables.begin(), it);
                    size_t offset = offsetof(ImGuiStyle, WindowMenuButtonPosition) + index * sizeof(ImGuiDir);
                    *reinterpret_cast<ImGuiDir *>(reinterpret_cast<char *>(&cStyle) + offset) = value;
                }
            }

            for (const auto&[key, value]: data.LogHoveredFlags) {
                auto it = std::find_if(ChangeLogs::AllVariables.begin(), ChangeLogs::AllVariables.end(),
                                       [&key](const std::pair<std::string, std::string>&p) {
                                           return p.first == key;
                                       });
                if (it != ChangeLogs::AllVariables.end()) {
                    size_t index = std::distance(ChangeLogs::AllVariables.begin(), it);
                    size_t offset = offsetof(ImGuiStyle, HoverFlagsForTooltipMouse) + index * sizeof(ImGuiHoveredFlags);
                    *reinterpret_cast<ImGuiHoveredFlags *>(reinterpret_cast<char *>(&cStyle) + offset) = value;
                }
            }

            for (const auto&[key, value]: data.LogBool) {
                auto it = std::find_if(ChangeLogs::AllVariables.begin(), ChangeLogs::AllVariables.end(),
                                       [&key](const std::pair<std::string, std::string>&p) {
                                           return p.first == key;
                                       });
                if (it != ChangeLogs::AllVariables.end()) {
                    size_t index = std::distance(ChangeLogs::AllVariables.begin(), it);
                    size_t offset = offsetof(ImGuiStyle, AntiAliasedLines) + index * sizeof(bool);
                    *reinterpret_cast<bool *>(reinterpret_cast<char *>(&cStyle) + offset) = value;
                }
            }

            ChangeLog = data;
        }

        void BeginFrame() override;

        void EndFrame() override;

        ImGuiStyle GetStyle() {
            return cStyle;
        }

        Style& operator=(const ImGuiStyle&data) {
            Modify(data);
            return *this;
        }

        Style& operator=(const Style&data) {
            Modify(data.cStyle);
            return *this;
        }

        bool DiffFrom(ImGuiStyle rhs) {
            bool changed = false;
            size_t CurrentOffset = 0;

            for (const auto&var: ChangeLogs::AllVariables) {
                const std::string&name = var.first;
                const std::string&type = var.second;

                if (type == "float") {
                    float lhsValue = *reinterpret_cast<float *>(reinterpret_cast<char *>(&cStyle) + CurrentOffset);
                    float rhsValue = *reinterpret_cast<float *>(reinterpret_cast<char *>(&rhs) + CurrentOffset);
                    if (std::abs(lhsValue - rhsValue) > EPSILON) {
                        ChangeLog.LogFloat[name] = lhsValue;
                        changed = true;
                    }
                    CurrentOffset += sizeof(float);
                }
                else if (type == "ImVec2") {
                    ImVec2 lhsValue = *reinterpret_cast<ImVec2 *>(reinterpret_cast<char *>(&cStyle) + CurrentOffset);
                    ImVec2 rhsValue = *reinterpret_cast<ImVec2 *>(reinterpret_cast<char *>(&rhs) + CurrentOffset);
                    if (lhsValue != rhsValue) {
                        ChangeLog.LogVec2[name] = lhsValue;
                        changed = true;
                    }
                    CurrentOffset += sizeof(ImVec2);
                }
                else if (type == "ImVec4[ImGuiCol_COUNT]") {
                    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
                        ImVec4 lhsValue = cStyle.Colors[i];
                        ImVec4 rhsValue = rhs.Colors[i];
                        if (lhsValue != rhsValue) {
                            //ChangeLog.LogVec4["Colors"][i] = lhsValue;
                            ChangeLog.LogColor[i] = lhsValue;
                            changed = true;
                        }
                    }
                    CurrentOffset += ImGuiCol_COUNT * sizeof(ImVec4);
                }
                else if (type == "ImGuiDir") {
                    ImGuiDir lhsValue = *reinterpret_cast<ImGuiDir *>(
                        reinterpret_cast<char *>(&cStyle) + CurrentOffset);
                    ImGuiDir rhsValue = *reinterpret_cast<ImGuiDir *>(reinterpret_cast<char *>(&rhs) + CurrentOffset);
                    if (lhsValue != rhsValue) {
                        ChangeLog.LogDir[name] = lhsValue;
                        changed = true;
                    }
                    CurrentOffset += sizeof(ImGuiDir);
                }
                else if (type == "bool") {
                    bool lhsValue = *reinterpret_cast<bool *>(reinterpret_cast<char *>(&cStyle) + CurrentOffset);
                    bool rhsValue = *reinterpret_cast<bool *>(reinterpret_cast<char *>(&rhs) + CurrentOffset);
                    if (lhsValue != rhsValue) {
                        ChangeLog.LogBool[name] = lhsValue;
                        changed = true;
                    }
                    CurrentOffset += sizeof(bool);
                }
                else if (type == "ImGuiHoveredFlags") {
                    ImGuiHoveredFlags lhsValue = *reinterpret_cast<ImGuiHoveredFlags *>(
                        reinterpret_cast<char *>(&cStyle) + CurrentOffset);
                    ImGuiHoveredFlags rhsValue = *reinterpret_cast<ImGuiHoveredFlags *>(
                        reinterpret_cast<char *>(&rhs) + CurrentOffset);
                    if (lhsValue != rhsValue) {
                        ChangeLog.LogHoveredFlags[name] = lhsValue;
                        changed = true;
                    }
                    CurrentOffset += sizeof(ImGuiHoveredFlags);
                }
            }

            return changed;
        }


        inline static ImGuiStyle DefaultStyle;
        ChangeLogs ChangeLog;

    private:
        ImGuiStyle cStyle;
    };
} // Mio

#endif //STYLE_H
