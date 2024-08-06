#ifndef GUIMANIFEST_H
#define GUIMANIFEST_H
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

#include "ResourceManager.h"
#include "Components/Event.h"
#include "Application.h"
#include <stdio.h>

namespace Mio {
    class ResourceManager;
    struct Manifests;

    class GUIManifest : public std::enable_shared_from_this<GUIManifest> {
    public:
        std::string SavePath;
        std::string sName;
        std::string sVersion = "m1.0.0";
        std::vector<std::shared_ptr<UIManager>> sManager;
        UUid sUUID = UUid::New();

        template<typename T>
        std::shared_ptr<T> GetUIByName(const std::string&name) {
            return ResourceManager::GetUIByName<T>(shared_from_this(), name);
        }

        template<typename T>
        std::shared_ptr<T> GetUIByUID(const UUid&uid) {
            return ResourceManager::GetUIByUID<T>(shared_from_this(), uid);
        }

        GUIManifest() = default;

        GUIManifest(const std::string&name) {
            sName = name;
        }

        static std::shared_ptr<GUIManifest> Create(const std::string&name) {
            return std::make_shared<GUIManifest>(name);
        }
    };
} // Mio


namespace YAML {
    template<typename T>
    struct convert<std::vector<T>> {
        static Node encode(const std::vector<T>&rhs) {
            Node node(NodeType::Sequence);
            for (const auto&item: rhs) {
                node.push_back(convert<T>::encode(item));
            }
            return node;
        }

        static bool decode(const Node&node, std::vector<T>&rhs) {
            if (!node.IsSequence()) {
                return false;
            }
            rhs.clear();
            for (const auto&item: node) {
                T value;
                if (!convert<T>::decode(item, value)) {
                    return false;
                }
                rhs.push_back(value);
            }
            return true;
        }
    };

    template<>
    struct convert<Mio::UIBase::Type> {
        static Node encode(const Mio::UIBase::Type&rhs) {
            Node node;
            switch (rhs) {
                case Mio::UIBase::Type::Button:
                    node = "Button";
                    break;
                case Mio::UIBase::Type::CheckBox:
                    node = "CheckBox";
                    break;
                case Mio::UIBase::Type::ColorEditor:
                    node = "ColorEditor";
                    break;
                case Mio::UIBase::Type::ColorPicker:
                    node = "ColorPicker";
                    break;
                case Mio::UIBase::Type::Combo:
                    node = "Combo";
                    break;
                case Mio::UIBase::Type::Image:
                    node = "Image";
                    break;
                case Mio::UIBase::Type::ImageButton:
                    node = "ImageButton";
                    break;
                case Mio::UIBase::Type::InputText:
                    node = "InputText";
                    break;
                case Mio::UIBase::Type::ListBox:
                    node = "ListBox";
                    break;
                case Mio::UIBase::Type::ProgressBar:
                    node = "ProgressBar";
                    break;
                case Mio::UIBase::Type::RadioButton:
                    node = "RadioButton";
                    break;
                case Mio::UIBase::Type::Slider:
                    node = "Slider";
                    break;
                case Mio::UIBase::Type::SubWindow:
                    node = "SubWindow";
                    break;
                case Mio::UIBase::Type::Text:
                    node = "Text";
                    break;
                case Mio::UIBase::Type::Tooltip:
                    node = "Tooltip";
                    break;
                case Mio::UIBase::Type::TreeNode:
                    node = "TreeNode";
                    break;
                case Mio::UIBase::Type::Window:
                    node = "Window";
                    break;
                case Mio::UIBase::Type::Popup:
                    node = "Popup";
                    break;
                case Mio::UIBase::Type::Group:
                    node = "Group";
                    break;
                default:
                    node = "None";
                    break;
            }
            return node;
        }

        static bool decode(const Node&node, Mio::UIBase::Type&rhs) {
            if (!node.IsScalar()) {
                return false;
            }
            std::string sType = node.as<std::string>();
            if (sType == "Button") {
                rhs = Mio::UIBase::Type::Button;
            }
            else if (sType == "CheckBox") {
                rhs = Mio::UIBase::Type::CheckBox;
            }
            else if (sType == "ColorEditor") {
                rhs = Mio::UIBase::Type::ColorEditor;
            }
            else if (sType == "ColorPicker") {
                rhs = Mio::UIBase::Type::ColorPicker;
            }
            else if (sType == "Combo") {
                rhs = Mio::UIBase::Type::Combo;
            }
            else if (sType == "Image") {
                rhs = Mio::UIBase::Type::Image;
            }
            else if (sType == "ImageButton") {
                rhs = Mio::UIBase::Type::ImageButton;
            }
            else if (sType == "InputText") {
                rhs = Mio::UIBase::Type::InputText;
            }
            else if (sType == "ListBox") {
                rhs = Mio::UIBase::Type::ListBox;
            }
            else if (sType == "ProgressBar") {
                rhs = Mio::UIBase::Type::ProgressBar;
            }
            else if (sType == "RadioButton") {
                rhs = Mio::UIBase::Type::RadioButton;
            }
            else if (sType == "Slider") {
                rhs = Mio::UIBase::Type::Slider;
            }
            else if (sType == "SubWindow") {
                rhs = Mio::UIBase::Type::SubWindow;
            }
            else if (sType == "Text") {
                rhs = Mio::UIBase::Type::Text;
            }
            else if (sType == "Tooltip") {
                rhs = Mio::UIBase::Type::Tooltip;
            }
            else if (sType == "TreeNode") {
                rhs = Mio::UIBase::Type::TreeNode;
            }
            else if (sType == "Window") {
                rhs = Mio::UIBase::Type::Window;
            }
            else if (sType == "Popup") {
                rhs = Mio::UIBase::Type::Popup;
            }
            else if (sType == "Group") {
                rhs = Mio::UIBase::Type::Group;
            }
            else {
                rhs = Mio::UIBase::Type::None;
            }
            return true;
        }
    };

    template<>
    struct convert<ImVec2> {
        static Node encode(const ImVec2&rhs) {
            Node node;
            node["x"] = rhs.x;
            node["y"] = rhs.y;
            return node;
        }

        static bool decode(const Node&node, ImVec2&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.x = node["x"].as<float>();
            rhs.y = node["y"].as<float>();
            return true;
        }
    };

    template<>
    struct convert<ImVec4> {
        static Node encode(const ImVec4&rhs) {
            Node node;
            node["x"] = rhs.x;
            node["y"] = rhs.y;
            node["z"] = rhs.z;
            node["w"] = rhs.w;
            return node;
        }

        static bool decode(const Node&node, ImVec4&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.x = node["x"].as<float>();
            rhs.y = node["y"].as<float>();
            rhs.z = node["z"].as<float>();
            rhs.w = node["w"].as<float>();
            return true;
        }
    };

    template<>
    struct convert<ImGuiStyle> {
        static Node encode(const ImGuiStyle&rhs) {
            Node node;
            node["Alpha"] = rhs.Alpha;
            node["DisabledAlpha"] = rhs.DisabledAlpha;
            node["WindowPadding"] = rhs.WindowPadding;
            node["WindowRounding"] = rhs.WindowRounding;
            node["WindowBorderSize"] = rhs.WindowBorderSize;
            node["WindowMinSize"] = rhs.WindowMinSize;
            node["WindowTitleAlign"] = rhs.WindowTitleAlign;
            node["WindowMenuButtonPosition"] = static_cast<int>(rhs.WindowMenuButtonPosition);
            node["ChildRounding"] = rhs.ChildRounding;
            node["ChildBorderSize"] = rhs.ChildBorderSize;
            node["PopupRounding"] = rhs.PopupRounding;
            node["PopupBorderSize"] = rhs.PopupBorderSize;
            node["FramePadding"] = rhs.FramePadding;
            node["FrameRounding"] = rhs.FrameRounding;
            node["FrameBorderSize"] = rhs.FrameBorderSize;
            node["ItemSpacing"] = rhs.ItemSpacing;
            node["ItemInnerSpacing"] = rhs.ItemInnerSpacing;
            node["CellPadding"] = rhs.CellPadding;
            node["TouchExtraPadding"] = rhs.TouchExtraPadding;
            node["IndentSpacing"] = rhs.IndentSpacing;
            node["ColumnsMinSpacing"] = rhs.ColumnsMinSpacing;
            node["ScrollbarSize"] = rhs.ScrollbarSize;
            node["ScrollbarRounding"] = rhs.ScrollbarRounding;
            node["GrabMinSize"] = rhs.GrabMinSize;
            node["GrabRounding"] = rhs.GrabRounding;
            node["LogSliderDeadzone"] = rhs.LogSliderDeadzone;
            node["TabRounding"] = rhs.TabRounding;
            node["TabBorderSize"] = rhs.TabBorderSize;
            node["TabMinWidthForCloseButton"] = rhs.TabMinWidthForCloseButton;
            node["TabBarBorderSize"] = rhs.TabBarBorderSize;
            node["TabBarOverlineSize"] = rhs.TabBarOverlineSize;
            node["TableAngledHeadersAngle"] = rhs.TableAngledHeadersAngle;
            node["TableAngledHeadersTextAlign"] = rhs.TableAngledHeadersTextAlign;
            node["ColorButtonPosition"] = static_cast<int>(rhs.ColorButtonPosition);
            node["ButtonTextAlign"] = rhs.ButtonTextAlign;
            node["SelectableTextAlign"] = rhs.SelectableTextAlign;
            node["SeparatorTextBorderSize"] = rhs.SeparatorTextBorderSize;
            node["SeparatorTextAlign"] = rhs.SeparatorTextAlign;
            node["SeparatorTextPadding"] = rhs.SeparatorTextPadding;
            node["DisplayWindowPadding"] = rhs.DisplayWindowPadding;
            node["DisplaySafeAreaPadding"] = rhs.DisplaySafeAreaPadding;
            node["DockingSeparatorSize"] = rhs.DockingSeparatorSize;
            node["MouseCursorScale"] = rhs.MouseCursorScale;
            node["AntiAliasedLines"] = rhs.AntiAliasedLines;
            node["AntiAliasedLinesUseTex"] = rhs.AntiAliasedLinesUseTex;
            node["AntiAliasedFill"] = rhs.AntiAliasedFill;
            node["CurveTessellationTol"] = rhs.CurveTessellationTol;
            node["CircleTessellationMaxError"] = rhs.CircleTessellationMaxError;

            Node colorsNode;
            for (int i = 0; i < ImGuiCol_COUNT; ++i) {
                colorsNode[std::to_string(i)] = rhs.Colors[i];
            }
            node["Colors"] = colorsNode;

            node["HoverStationaryDelay"] = rhs.HoverStationaryDelay;
            node["HoverDelayShort"] = rhs.HoverDelayShort;
            node["HoverDelayNormal"] = rhs.HoverDelayNormal;
            node["HoverFlagsForTooltipMouse"] = rhs.HoverFlagsForTooltipMouse;
            node["HoverFlagsForTooltipNav"] = rhs.HoverFlagsForTooltipNav;

            return node;
        }

        static bool decode(const Node&node, ImGuiStyle&rhs) {
            if (!node.IsMap()) {
                return false;
            }

            rhs.Alpha = node["Alpha"].as<float>();
            rhs.DisabledAlpha = node["DisabledAlpha"].as<float>();
            rhs.WindowPadding = node["WindowPadding"].as<ImVec2>();
            rhs.WindowRounding = node["WindowRounding"].as<float>();
            rhs.WindowBorderSize = node["WindowBorderSize"].as<float>();
            rhs.WindowMinSize = node["WindowMinSize"].as<ImVec2>();
            rhs.WindowTitleAlign = node["WindowTitleAlign"].as<ImVec2>();
            rhs.WindowMenuButtonPosition = static_cast<ImGuiDir>(node["WindowMenuButtonPosition"].as<int>());
            rhs.ChildRounding = node["ChildRounding"].as<float>();
            rhs.ChildBorderSize = node["ChildBorderSize"].as<float>();
            rhs.PopupRounding = node["PopupRounding"].as<float>();
            rhs.PopupBorderSize = node["PopupBorderSize"].as<float>();
            rhs.FramePadding = node["FramePadding"].as<ImVec2>();
            rhs.FrameRounding = node["FrameRounding"].as<float>();
            rhs.FrameBorderSize = node["FrameBorderSize"].as<float>();
            rhs.ItemSpacing = node["ItemSpacing"].as<ImVec2>();
            rhs.ItemInnerSpacing = node["ItemInnerSpacing"].as<ImVec2>();
            rhs.CellPadding = node["CellPadding"].as<ImVec2>();
            rhs.TouchExtraPadding = node["TouchExtraPadding"].as<ImVec2>();
            rhs.IndentSpacing = node["IndentSpacing"].as<float>();
            rhs.ColumnsMinSpacing = node["ColumnsMinSpacing"].as<float>();
            rhs.ScrollbarSize = node["ScrollbarSize"].as<float>();
            rhs.ScrollbarRounding = node["ScrollbarRounding"].as<float>();
            rhs.GrabMinSize = node["GrabMinSize"].as<float>();
            rhs.GrabRounding = node["GrabRounding"].as<float>();
            rhs.LogSliderDeadzone = node["LogSliderDeadzone"].as<float>();
            rhs.TabRounding = node["TabRounding"].as<float>();
            rhs.TabBorderSize = node["TabBorderSize"].as<float>();
            rhs.TabMinWidthForCloseButton = node["TabMinWidthForCloseButton"].as<float>();
            rhs.TabBarBorderSize = node["TabBarBorderSize"].as<float>();
            rhs.TabBarOverlineSize = node["TabBarOverlineSize"].as<float>();
            rhs.TableAngledHeadersAngle = node["TableAngledHeadersAngle"].as<float>();
            rhs.TableAngledHeadersTextAlign = node["TableAngledHeadersTextAlign"].as<ImVec2>();
            rhs.ColorButtonPosition = static_cast<ImGuiDir>(node["ColorButtonPosition"].as<int>());
            rhs.ButtonTextAlign = node["ButtonTextAlign"].as<ImVec2>();
            rhs.SelectableTextAlign = node["SelectableTextAlign"].as<ImVec2>();
            rhs.SeparatorTextBorderSize = node["SeparatorTextBorderSize"].as<float>();
            rhs.SeparatorTextAlign = node["SeparatorTextAlign"].as<ImVec2>();
            rhs.SeparatorTextPadding = node["SeparatorTextPadding"].as<ImVec2>();
            rhs.DisplayWindowPadding = node["DisplayWindowPadding"].as<ImVec2>();
            rhs.DisplaySafeAreaPadding = node["DisplaySafeAreaPadding"].as<ImVec2>();
            rhs.DockingSeparatorSize = node["DockingSeparatorSize"].as<float>();
            rhs.MouseCursorScale = node["MouseCursorScale"].as<float>();
            rhs.AntiAliasedLines = node["AntiAliasedLines"].as<bool>();
            rhs.AntiAliasedLinesUseTex = node["AntiAliasedLinesUseTex"].as<bool>();
            rhs.AntiAliasedFill = node["AntiAliasedFill"].as<bool>();
            rhs.CurveTessellationTol = node["CurveTessellationTol"].as<float>();
            rhs.CircleTessellationMaxError = node["CircleTessellationMaxError"].as<float>();

            const Node&colorsNode = node["Colors"];
            for (int i = 0; i < ImGuiCol_COUNT; ++i) {
                rhs.Colors[i] = colorsNode[std::to_string(i)].as<ImVec4>();
            }

            rhs.HoverStationaryDelay = node["HoverStationaryDelay"].as<float>();
            rhs.HoverDelayShort = node["HoverDelayShort"].as<float>();
            rhs.HoverDelayNormal = node["HoverDelayNormal"].as<float>();
            rhs.HoverFlagsForTooltipMouse = node["HoverFlagsForTooltipMouse"].as<ImGuiHoveredFlags>();
            rhs.HoverFlagsForTooltipNav = node["HoverFlagsForTooltipNav"].as<ImGuiHoveredFlags>();

            return true;
        }
    };

    template<>
    struct convert<Mio::ChangeLogs> {
        static Node encode(const Mio::ChangeLogs&rhs) {
            Node node;

            for (const auto&[key, value]: rhs.LogFloat) {
                node[key] = value;
            }

            for (const auto&[key, value]: rhs.LogVec2) {
                node[key] = YAML::Load("[" + std::to_string(value.x) + ", " + std::to_string(value.y) + "]");
            }

            for (const auto&[key, value]: rhs.LogVec4) {
                Node colors;
                for (int i = 0; i < ImGuiCol_COUNT; ++i) {
                    colors.push_back(YAML::Load(
                        "[" + std::to_string(value[i].x) + ", " + std::to_string(value[i].y) + ", " +
                        std::to_string(value[i].z) + ", " + std::to_string(value[i].w) + "]"));
                }
                node[key] = colors;
            }

            for (const auto&[key, value]: rhs.LogDir) {
                node[key] = static_cast<int>(value);
            }

            for (const auto&[key, value]: rhs.LogHoveredFlags) {
                node[key] = static_cast<int>(value);
            }

            for (const auto&[key, value]: rhs.LogBool) {
                node[key] = value;
            }

            return node;
        }

        static bool decode(const Node&node, Mio::ChangeLogs&rhs) {
            if (!node.IsMap()) {
                return false;
            }

            for (const auto&it: node) {
                const std::string&key = it.first.as<std::string>();
                const Node&valueNode = it.second;

                try {
                    float floatValue = valueNode.as<float>();
                    rhs.LogFloat[key] = floatValue;
                }
                catch (const YAML::BadConversion&) {
                    try {
                        int intValue = valueNode.as<int>();
                        if (intValue >= 0 && intValue <= 4) {
                            rhs.LogDir[key] = static_cast<ImGuiDir>(intValue);
                        }
                        else {
                            rhs.LogHoveredFlags[key] = static_cast<ImGuiHoveredFlags>(intValue);
                        }
                    }
                    catch (const YAML::BadConversion&) {
                        try {
                            bool boolValue = valueNode.as<bool>();
                            rhs.LogBool[key] = boolValue;
                        }
                        catch (const YAML::BadConversion&) {
                            if (valueNode.IsSequence()) {
                                if (valueNode.size() == 2) {
                                    rhs.LogVec2[key] = ImVec2(valueNode[0].as<float>(), valueNode[1].as<float>());
                                }
                                else if (valueNode.size() == 4 && key == "Colors") {
                                    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
                                        rhs.LogVec4[key][i] = ImVec4(valueNode[i][0].as<float>(),
                                                                     valueNode[i][1].as<float>(),
                                                                     valueNode[i][2].as<float>(),
                                                                     valueNode[i][3].as<float>());
                                    }
                                }
                            }
                        }
                    }
                }
            }

            return true;
        }
    };

    template<>
    struct convert<Mio::Transform> {
        static Node encode(const Mio::Transform&rhs) {
            Node node;
            node["position"] = rhs.cPosition;
            node["scale"] = rhs.cScale;
            node["size"] = rhs.cSize;
            return node;
        }

        static bool decode(const Node&node, Mio::Transform&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.cPosition = node["position"].as<ImVec2>();
            rhs.cScale = node["scale"].as<ImVec2>();
            rhs.cSize = node["size"].as<ImVec2>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Style> {
        static Node encode(Mio::Style rhs) {
            Node node;
            node["style"] = rhs.GetStyle();
            return node;
        }

        static bool decode(const Node&node, Mio::Style&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.Modify(node["style"].as<ImGuiStyle>());
            return true;
        }
    };

    template<>
    struct convert<Mio::Event> {
        static Node encode(const Mio::Event&rhs) {
            Node node;
            node["funcs"] = rhs.funcs;
            return node;
        }

        static bool decode(const Node&node, Mio::Event&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.funcs = node["funcs"].as<std::vector<std::string>>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Button::Data> {
        static Node encode(const Mio::Button::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            return node;
        }

        static bool decode(const Node&node, Mio::Button::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            return true;
        }
    };

    template<>
    struct convert<Mio::ImageButton::Data> {
        static Node encode(const Mio::ImageButton::Data&rhs) {
            Node node;
            node["imagePath"] = rhs.imagePath;
            node["uv0"] = rhs.uv0;
            node["uv1"] = rhs.uv1;
            node["frame_padding"] = rhs.frame_padding;
            node["bg_col"] = rhs.bg_col;
            node["tint_col"] = rhs.tint_col;
            return node;
        }

        static bool decode(const Node&node, Mio::ImageButton::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.imagePath = node["imagePath"].as<std::string>();
            rhs.uv0 = node["uv0"].as<ImVec2>();
            rhs.uv1 = node["uv1"].as<ImVec2>();
            rhs.frame_padding = node["frame_padding"].as<int>();
            rhs.bg_col = node["bg_col"].as<ImVec4>();
            rhs.tint_col = node["tint_col"].as<ImVec4>();
            return true;
        }
    };

    template<>
    struct convert<Mio::CheckBox::Data> {
        static Node encode(const Mio::CheckBox::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["value"] = *rhs.value;
            return node;
        }

        static bool decode(const Node&node, Mio::CheckBox::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            *rhs.value = node["value"].as<bool>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Text::Data> {
        static Node encode(const Mio::Text::Data&rhs) {
            Node node;
            node["text"] = rhs.text;
            node["text_size"] = rhs.text_size;
            node["outline_thickness"] = rhs.outline_thickness;
            node["fill_color"] = rhs.fill_color;
            node["outline_color"] = rhs.outline_color;
            return node;
        }

        static bool decode(const Node&node, Mio::Text::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.text = node["text"].as<std::string>();
            rhs.text_size = node["text_size"].as<ImVec2>();
            rhs.outline_thickness = node["outline_thickness"].as<ImVec2>();
            rhs.fill_color = node["fill_color"].as<ImU32>();
            rhs.outline_color = node["outline_color"].as<ImU32>();
            return true;
        }
    };

    template<>
    struct convert<Mio::InputText::Data> {
        static Node encode(const Mio::InputText::Data&rhs) {
            Node node;
            node["label"] = rhs.label;
            node["buf"] = std::string(rhs.buf);
            node["buf_size"] = rhs.buf_size;
            node["hint"] = rhs.hint;
            node["Multiline"] = rhs.Multiline;
            node["flags"] = rhs.flags;
            return node;
        }

        static bool decode(const Node&node, Mio::InputText::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.label = node["label"].as<std::string>().c_str();
            std::string buf = node["buf"].as<std::string>();
            rhs.buf = new char[buf.size() + 1];
            strcpy(rhs.buf, buf.c_str());
            rhs.buf_size = node["buf_size"].as<size_t>();
            rhs.hint = node["hint"].as<std::string>().c_str();
            rhs.Multiline = node["Multiline"].as<bool>();
            rhs.flags = node["flags"].as<ImGuiInputTextFlags>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Image::Data> {
        static Node encode(const Mio::Image::Data&rhs) {
            Node node;
            node["imagePath"] = rhs.imagePath;
            node["uv0"] = rhs.uv0;
            node["uv1"] = rhs.uv1;
            node["frame_padding"] = rhs.frame_padding;
            node["bg_col"] = rhs.bg_col;
            node["tint_col"] = rhs.tint_col;
            node["border_col"] = rhs.border_col;
            return node;
        }

        static bool decode(const Node&node, Mio::Image::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.imagePath = node["imagePath"].as<std::string>();
            rhs.uv0 = node["uv0"].as<ImVec2>();
            rhs.uv1 = node["uv1"].as<ImVec2>();
            rhs.frame_padding = node["frame_padding"].as<int>();
            rhs.bg_col = node["bg_col"].as<ImVec4>();
            rhs.tint_col = node["tint_col"].as<ImVec4>();
            rhs.border_col = node["border_col"].as<ImVec4>();
            return true;
        }
    };

    template<>
    struct convert<Mio::RadioButton::Data> {
        static Node encode(const Mio::RadioButton::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["v"] = *rhs.v;
            node["v_button"] = rhs.v_button;
            return node;
        }

        static bool decode(const Node&node, Mio::RadioButton::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            *rhs.v = node["v"].as<int>();
            rhs.v_button = node["v_button"].as<int>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Combo::Data> {
        static Node encode(const Mio::Combo::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["preview_value"] = rhs.preview_value;
            node["items"] = rhs.items;
            node["current_item"] = *rhs.current_item;
            node["flags"] = rhs.flags;
            return node;
        }

        static bool decode(const Node&node, Mio::Combo::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.preview_value = node["preview_value"].as<std::string>().c_str();
            rhs.items = node["items"].as<std::vector<std::string>>();
            *rhs.current_item = node["current_item"].as<int>();
            rhs.flags = node["flags"].as<ImGuiComboFlags>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Slider::Data> {
        static Node encode(const Mio::Slider::Data&rhs) {
            Node node;
            node["label"] = rhs.label;
            node["type"] = static_cast<int>(rhs.type);
            node["components"] = rhs.components;
            node["v_min"] = rhs.v_min;
            node["v_max"] = rhs.v_max;
            node["format"] = rhs.format;
            node["flags"] = rhs.flags;
            return node;
        }

        static bool decode(const Node&node, Mio::Slider::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.label = node["label"].as<std::string>().c_str();
            rhs.type = static_cast<Mio::Slider::Type>(node["type"].as<int>());

            rhs.components = node["components"].as<int>();
            rhs.v_min = node["v_min"].as<float>();
            rhs.v_max = node["v_max"].as<float>();
            rhs.format = node["format"].as<std::string>().c_str();
            rhs.flags = node["flags"].as<ImGuiSliderFlags>();
            return true;
        }
    };

    template<>
    struct convert<Mio::ColorPicker::Data> {
        static Node encode(const Mio::ColorPicker::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            //node["v"] = *rhs.v;
            node["flags"] = rhs.flags;
            node["ref_col"] = rhs.ref_col;
            return node;
        }

        static bool decode(const Node&node, Mio::ColorPicker::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.flags = node["flags"].as<ImGuiColorEditFlags>();
            rhs.ref_col = node["ref_col"].as<ImVec4>();
            return true;
        }
    };

    template<>
    struct convert<Mio::ColorEditor::Data> {
        static Node encode(const Mio::ColorEditor::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["v"] = *rhs.v;
            node["flags"] = rhs.flags;
            return node;
        }

        static bool decode(const Node&node, Mio::ColorEditor::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            *rhs.v = node["v"].as<ImVec4>();
            rhs.flags = node["flags"].as<ImGuiColorEditFlags>();
            return true;
        }
    };

    template<>
    struct convert<Mio::ProgressBar::Data> {
        static Node encode(const Mio::ProgressBar::Data&rhs) {
            Node node;
            node["progress"] = *rhs.progress;
            node["overlay"] = rhs.overlay;
            return node;
        }

        static bool decode(const Node&node, Mio::ProgressBar::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            *rhs.progress = node["progress"].as<float>();
            rhs.overlay = node["overlay"].as<std::string>().c_str();
            return true;
        }
    };

    template<>
    struct convert<Mio::TreeNode::Data> {
        static Node encode(const Mio::TreeNode::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["flags"] = rhs.flags;
            return node;
        }

        static bool decode(const Node&node, Mio::TreeNode::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.flags = node["flags"].as<ImGuiTreeNodeFlags>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Tooltip::Data> {
        static Node encode(const Mio::Tooltip::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            return node;
        }

        static bool decode(const Node&node, Mio::Tooltip::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            return true;
        }
    };

    template<>
    struct convert<Mio::ListBox::Data> {
        static Node encode(const Mio::ListBox::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["preview_value"] = rhs.preview_value;
            node["items"] = rhs.items;
            node["current_item"] = *rhs.current_item;
            node["flags"] = rhs.flags;
            return node;
        }

        static bool decode(const Node&node, Mio::ListBox::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.preview_value = node["preview_value"].as<std::string>().c_str();
            rhs.items = node["items"].as<std::vector<std::string>>();
            *rhs.current_item = node["current_item"].as<int>();
            rhs.flags = node["flags"].as<ImGuiComboFlags>();
            return true;
        }
    };

    template<>
    struct convert<Mio::SubWindow::Data> {
        static Node encode(const Mio::SubWindow::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["size"] = rhs.size;
            node["child_flags"] = rhs.child_flags;
            node["window_flags"] = rhs.window_flags;
            return node;
        }

        static bool decode(const Node&node, Mio::SubWindow::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.size = node["size"].as<ImVec2>();
            rhs.child_flags = node["child_flags"].as<ImGuiChildFlags>();
            rhs.window_flags = node["window_flags"].as<ImGuiWindowFlags>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Window::Data> {
        static Node encode(const Mio::Window::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["p_open"] = rhs.p_open;
            node["flags"] = rhs.flags;
            node["fullScreen"] = rhs.fullScreen;
            return node;
        }

        static bool decode(const Node&node, Mio::Window::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.p_open = node["p_open"].as<bool>();
            rhs.flags = node["flags"].as<ImGuiWindowFlags>();
            rhs.fullScreen = node["fullScreen"].as<bool>();
            return true;
        }
    };

    template<>
    struct convert<Mio::MenuItem::Data> {
        static Node encode(const Mio::MenuItem::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["enabled"] = rhs.enabled;
            node["shortcut"] = rhs.shortcut;
            return node;
        }

        static bool decode(const Node&node, Mio::MenuItem::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.enabled = node["enabled"].as<bool>();
            rhs.shortcut = node["shortcut"].as<std::string>().c_str();
            return true;
        }
    };

    template<>
    struct convert<Mio::MenuBar::Data> {
        static Node encode(const Mio::MenuBar::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["enabled"] = rhs.enabled;
            return node;
        }

        static bool decode(const Node&node, Mio::MenuBar::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.enabled = node["enabled"].as<bool>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Group::Data> {
        static Node encode(const Mio::Group::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            return node;
        }

        static bool decode(const Node&node, Mio::Group::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            return true;
        }
    };

    template<>
    struct convert<Mio::Popup::Data> {
        static Node encode(const Mio::Popup::Data&rhs) {
            Node node;
            node["name"] = rhs.name;
            node["flags"] = rhs.flags;
            return node;
        }

        static bool decode(const Node&node, Mio::Popup::Data&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.name = node["name"].as<std::string>();
            rhs.flags = node["flags"].as<ImGuiWindowFlags>();
            return true;
        }
    };

    template<>
    struct convert<std::shared_ptr<Mio::MenuItem>> {
        static Node encode(const std::shared_ptr<Mio::MenuItem>&rhs) {
            Node node;
            node["data"] = rhs->cData;
            node["UID"] = rhs->uuid;
            node["name"] = rhs->cName;
            return node;
        }

        static bool decode(const Node&node, std::shared_ptr<Mio::MenuItem>&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs->uuid = node["UID"].as<Mio::UUid>();
            rhs->cName = node["name"].as<std::string>();
            rhs = std::make_shared<Mio::MenuItem>(node["data"].as<Mio::MenuItem::Data>(), rhs->cName);
            return true;
        }
    };

    template<>
    struct convert<Mio::UUid> {
        static Node encode(const Mio::UUid&rhs) {
            Node node;
            auto data = rhs.GetData();
            for (auto part: data) {
                node.push_back(part);
            }
            return node;
        }

        static bool decode(const Node&node, Mio::UUid&rhs) {
            if (node.size() != 4) return false;

            std::array<uint32_t, 4> data;
            for (size_t i = 0; i < node.size(); ++i) {
                data[i] = node[i].as<uint32_t>();
            }
            rhs.SetData(data);
            return true;
        }
    };

    template<>
    struct convert<std::shared_ptr<Mio::UIBase>> {
        static Node encode(std::shared_ptr<Mio::UIBase> rhs) {
            Node node;
            node["type"] = rhs->cType;
            node["name"] = rhs->cName;
            node["UID"] = rhs->UID();
            node["transform"] = rhs->transform;
            node["style"] = rhs->style.ChangeLog;
            node["refVariables"] = rhs->RefVariables;
            switch (rhs->cType) {
                case Mio::UIBase::Type::Button: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::Button>(rhs)->cData;
                    node["events"] = std::reinterpret_pointer_cast<Mio::Button>(rhs)->GetComponent<Mio::Event>().
                            GetFuncs();
                    break;
                }
                case Mio::UIBase::Type::CheckBox: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::CheckBox>(rhs)->cData;
                    node["events"] = std::reinterpret_pointer_cast<Mio::CheckBox>(rhs)->GetComponent<Mio::Event>().
                            GetFuncs();
                    break;
                }
                case Mio::UIBase::Type::ColorEditor: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::ColorEditor>(rhs)->cData;
                    break;
                }
                case Mio::UIBase::Type::ColorPicker: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::ColorPicker>(rhs)->cData;
                    break;
                }
                case Mio::UIBase::Type::Combo: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::Combo>(rhs)->cData;
                    break;
                }
                case Mio::UIBase::Type::Image: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::Image>(rhs)->cData;
                    break;
                }
                case Mio::UIBase::Type::ImageButton: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::ImageButton>(rhs)->cData;
                    node["events"] = std::reinterpret_pointer_cast<Mio::ImageButton>(rhs)->GetComponent<Mio::Event>().
                            GetFuncs();
                    break;
                }
                case Mio::UIBase::Type::InputText: {
                    auto inputText = std::reinterpret_pointer_cast<Mio::InputText>(rhs);
                    node["data"] = inputText->cData;
                    node["events"] = inputText->GetComponent<Mio::Event>().GetFuncs();
                    break;
                }
                case Mio::UIBase::Type::ListBox: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::ListBox>(rhs)->cData;
                    break;
                }
                case Mio::UIBase::Type::ProgressBar: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::ProgressBar>(rhs)->cData;
                    break;
                }
                case Mio::UIBase::Type::RadioButton: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::RadioButton>(rhs)->cData;
                    break;
                }
                case Mio::UIBase::Type::Slider: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::Slider>(rhs)->cData;
                    node["events"] = std::reinterpret_pointer_cast<Mio::Slider>(rhs)->GetComponent<Mio::Event>().
                            GetFuncs();
                    break;
                }
                case Mio::UIBase::Type::SubWindow: {
                    auto subWindow = std::reinterpret_pointer_cast<Mio::SubWindow>(rhs);
                    node["data"] = subWindow->cData;
                    for (auto&uiElement: subWindow->uiElements) {
                        node["uiElements"].push_back(encode(uiElement));
                    }

                    break;
                }
                case Mio::UIBase::Type::Window: {
                    auto window = std::reinterpret_pointer_cast<Mio::Window>(rhs);
                    node["data"] = window->cData;
                    for (const auto&uiElement: window->uiElements) {
                        if (uiElement != nullptr) {
                            node["uiElements"].push_back(encode(uiElement));
                        }
                    }
                    break;
                }
                case Mio::UIBase::Type::Text: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::Text>(rhs)->cData;
                    break;
                }

                case Mio::UIBase::Type::TreeNode: {
                    auto treeNode = std::reinterpret_pointer_cast<Mio::TreeNode>(rhs);
                    node["data"] = treeNode->cData;
                    for (auto&uiElement: treeNode->uiElements) {
                        node["uiElements"].push_back(encode(uiElement));
                    }

                    break;
                }
                case Mio::UIBase::Type::Tooltip: {
                    auto tooltip = std::reinterpret_pointer_cast<Mio::Tooltip>(rhs);
                    node["data"] = std::reinterpret_pointer_cast<Mio::Tooltip>(rhs)->cData;
                    for (auto&uiElement: tooltip->uiElements) {
                        node["uiElements"].push_back(encode(uiElement));
                    }
                    break;
                }
                case Mio::UIBase::Type::MenuBar: {
                    auto menuBar = std::reinterpret_pointer_cast<Mio::MenuBar>(rhs);
                    node["data"] = std::reinterpret_pointer_cast<Mio::MenuBar>(rhs)->cData;
                    for (auto&menuItem: menuBar->menuItems) {
                        node["menuItems"].push_back(encode(menuItem));
                    }
                    break;
                }
                case Mio::UIBase::Type::Group: {
                    auto group = std::reinterpret_pointer_cast<Mio::Group>(rhs);
                    node["data"] = std::reinterpret_pointer_cast<Mio::Group>(rhs)->cData;
                    for (auto&uiElement: group->uiElements) {
                        node["uiElements"].push_back(encode(uiElement));
                    }
                    break;
                }
                case Mio::UIBase::Type::Popup: {
                    auto popup = std::reinterpret_pointer_cast<Mio::Popup>(rhs);
                    node["data"] = std::reinterpret_pointer_cast<Mio::Popup>(rhs)->cData;
                    for (auto&uiElement: popup->uiElements) {
                        node["uiElements"].push_back(encode(uiElement));
                    }
                    break;
                }
                case Mio::UIBase::Type::MenuItem: {
                    node["data"] = std::reinterpret_pointer_cast<Mio::MenuItem>(rhs)->cData;
                    node["funcs"] = std::reinterpret_pointer_cast<Mio::MenuItem>(rhs)->GetComponent<Mio::Event>().
                            GetFuncs();
                    break;
                }
                default:
                    break;
            }
            return node;
        }

        static bool decode(const Node&node, std::shared_ptr<Mio::UIBase>&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            const auto type = node["type"].as<Mio::UIBase::Type>();
            rhs->cName = node["name"].as<std::string>();
            switch (type) {
                case Mio::UIBase::Type::Button:
                    rhs = Mio::Button::Create(node["data"].as<Mio::Button::Data>());
                    rhs->GetComponent<Mio::Event>().SetFuncs(node["events"].as<std::vector<std::string>>());
                    break;
                case Mio::UIBase::Type::CheckBox:
                    rhs = Mio::CheckBox::Create(node["data"].as<Mio::CheckBox::Data>(), rhs->cName);
                    rhs->GetComponent<Mio::Event>().SetFuncs(node["events"].as<std::vector<std::string>>());
                    break;
                case Mio::UIBase::Type::ColorEditor:
                    rhs = Mio::ColorEditor::Create(node["data"].as<Mio::ColorEditor::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::ColorPicker:
                    rhs = Mio::ColorPicker::Create(node["data"].as<Mio::ColorPicker::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::Combo:
                    rhs = Mio::Combo::Create(node["data"].as<Mio::Combo::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::Image:
                    rhs = Mio::Image::Create(node["data"].as<Mio::Image::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::ImageButton:
                    rhs = Mio::ImageButton::Create(node["data"].as<Mio::ImageButton::Data>(), rhs->cName);
                    rhs->GetComponent<Mio::Event>().SetFuncs(node["events"].as<std::vector<std::string>>());
                    break;
                case Mio::UIBase::Type::InputText:
                    rhs = Mio::InputText::Create(node["data"].as<Mio::InputText::Data>(), rhs->cName);
                    rhs->GetComponent<Mio::Event>().SetFuncs(node["events"].as<std::vector<std::string>>());
                    break;
                case Mio::UIBase::Type::ListBox:
                    rhs = Mio::ListBox::Create(node["data"].as<Mio::ListBox::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::ProgressBar:
                    rhs = Mio::ProgressBar::Create(node["data"].as<Mio::ProgressBar::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::RadioButton:
                    rhs = Mio::RadioButton::Create(node["data"].as<Mio::RadioButton::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::Slider:
                    rhs = Mio::Slider::Create(node["data"].as<Mio::Slider::Data>(), rhs->cName);
                    rhs->GetComponent<Mio::Event>().SetFuncs(node["events"].as<std::vector<std::string>>());
                case Mio::UIBase::Type::Text:
                    rhs =
                            Mio::Text::Create(node["data"].as<Mio::Text::Data>(), rhs->cName);
                    break;
                case Mio::UIBase::Type::Tooltip:
                    rhs = Mio::Tooltip::Create(node["data"].as<Mio::Tooltip::Data>(), rhs->cName);
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        std::dynamic_pointer_cast<Mio::Tooltip>(rhs)->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::TreeNode:
                    rhs = Mio::TreeNode::Create(node["data"].as<Mio::TreeNode::Data>(), rhs->cName);
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        std::dynamic_pointer_cast<Mio::TreeNode>(rhs)->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::Window:
                    rhs = Mio::Window::Create(node["data"].as<Mio::Window::Data>(), rhs->cName);
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        std::dynamic_pointer_cast<Mio::Window>(rhs)->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::MenuBar:
                    rhs = Mio::MenuBar::Create(node["data"].as<Mio::MenuBar::Data>(), rhs->cName);
                    for (const auto&element: node["menuItems"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        std::dynamic_pointer_cast<Mio::MenuBar>(rhs)->menuItems.push_back(
                            std::dynamic_pointer_cast<Mio::MenuItem>(uiElement));
                    }
                    break;
                case Mio::UIBase::Type::Group:
                    rhs = Mio::Group::Create(node["data"].as<Mio::Group::Data>());
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        std::dynamic_pointer_cast<Mio::Group>(rhs)->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::Popup:
                    rhs = Mio::Popup::Create(node["data"].as<Mio::Popup::Data>());
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        std::dynamic_pointer_cast<Mio::Popup>(rhs)->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::MenuItem:
                    rhs = Mio::MenuItem::Create(node["data"].as<Mio::MenuItem::Data>(), rhs->cName);
                    rhs->GetComponent<Mio::Event>().SetFuncs(node["events"].as<std::vector<std::string>>());
                    break;
                default:
                    return false;
                    break;
            }

            rhs->uuid = node["UID"].as<Mio::UUid>();
            if (node["RefVariables"].IsDefined() && !node["RefVariables"].IsNull()) {
                rhs->RefVariables = node["RefVariables"].as<std::vector<std::string>>();
            }

            rhs->GetComponent<Mio::Transform>().SetTransform(node["transform"].as<Mio::Transform>());
            if (node["style"].IsDefined() && !node["style"].IsNull()) {
                rhs->GetComponent<Mio::Style>().Modify(node["style"].as<Mio::ChangeLogs>());
            }

            return true;
        }
    };

    template<>
    struct convert<std::vector<std::shared_ptr<Mio::UIBase>>> {
        static Node encode(const std::vector<std::shared_ptr<Mio::UIBase>>&rhs) {
            Node node(NodeType::Sequence);
            for (const auto&element: rhs) {
                node.push_back(convert<std::shared_ptr<Mio::UIBase>>::encode(element));
            }
            return node;
        }

        static bool decode(const Node&node, std::vector<std::shared_ptr<Mio::UIBase>>&rhs) {
            if (!node.IsSequence()) {
                return false;
            }
            rhs.clear();
            for (const auto&element: node["uiElements"]) {
                std::shared_ptr<Mio::UIBase> uiElement;
                if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                    return false;
                }
                rhs.push_back(uiElement);
            }
            return true;
        }
    };

    template<>
    struct convert<std::shared_ptr<Mio::UIManager>> {
        static Node encode(const std::shared_ptr<Mio::UIBase>&rhs) {
            Node node;
            node = convert<std::shared_ptr<Mio::UIBase>>::encode(rhs);
            return node;
        }

        static bool decode(const Node&node, std::shared_ptr<Mio::UIManager>&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            const auto type = node["type"].as<Mio::UIBase::Type>();

            switch (type) {
                case Mio::UIBase::Type::Tooltip:
                    rhs = std::dynamic_pointer_cast<Mio::Tooltip>(
                        Mio::Tooltip::Create(node["data"].as<Mio::Tooltip::Data>(), rhs->cName));
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        rhs->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::TreeNode:
                    rhs = std::dynamic_pointer_cast<Mio::TreeNode>(
                        Mio::TreeNode::Create(node["data"].as<Mio::TreeNode::Data>(), rhs->cName));
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        rhs->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::Window:
                    rhs = std::dynamic_pointer_cast<Mio::Window>(
                        Mio::Window::Create(node["data"].as<Mio::Window::Data>(), rhs->cName));
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        rhs->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::SubWindow:
                    rhs = std::dynamic_pointer_cast<Mio::SubWindow>(
                        Mio::SubWindow::Create(node["data"].as<Mio::SubWindow::Data>(), rhs->cName));
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        rhs->uiElements.push_back(uiElement);
                    }
                case Mio::UIBase::Type::MenuBar: rhs = Mio::MenuBar::Create(
                                                     node["data"].as<Mio::MenuBar::Data>(), rhs->cName);
                    for (const auto&element: node["menuItems"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        std::dynamic_pointer_cast<Mio::MenuBar>(rhs)->menuItems.push_back(
                            std::dynamic_pointer_cast<Mio::MenuItem>(uiElement));
                    }
                    break;
                case Mio::UIBase::Type::Group:
                    rhs = Mio::Group::Create(node["data"].as<Mio::Group::Data>());
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        rhs->uiElements.push_back(uiElement);
                    }
                    break;
                case Mio::UIBase::Type::Popup:
                    rhs = Mio::Popup::Create(node["data"].as<Mio::Popup::Data>());
                    for (const auto&element: node["uiElements"]) {
                        std::shared_ptr<Mio::UIBase> uiElement = std::make_shared<Mio::UIBase>();
                        if (!convert<std::shared_ptr<Mio::UIBase>>::decode(element, uiElement)) {
                            return false;
                        }
                        rhs->uiElements.push_back(uiElement);
                    }

                default:
                    return false;
                    break;
            }
            rhs->GetComponent<Mio::Transform>().SetTransform(node["transform"].as<Mio::Transform>());
            if (node["style"].IsDefined() && !node["style"].IsNull()) {
                rhs->GetComponent<Mio::Style>().Modify(node["style"].as<Mio::ChangeLogs>());
            }
            rhs->cType = type;
            rhs->cName = node["name"].as<std::string>();
            rhs->uuid = node["UID"].as<Mio::UUid>();
            return true;
        }
    };

    template<>
    struct convert<Mio::GUIManifest> {
        static Node encode(const Mio::GUIManifest&rhs) {
            Node node;
            node["name"] = rhs.sName;
            node["version"] = rhs.sVersion;
            node["Managers"] = rhs.sManager;
            node["uid"] = rhs.sUUID;
            node["SavePath"] = rhs.SavePath;
            return node;
        }

        static bool decode(const Node&node, Mio::GUIManifest&rhs) {
            if (!node.IsMap()) {
                return false;
            }
            rhs.sName = node["name"].as<std::string>();
            rhs.sVersion = node["version"].as<std::string>();
            for (const auto&element: node["Managers"]) {
                std::shared_ptr<Mio::UIManager> uiManager = std::make_shared<Mio::UIManager>();
                if (!convert<std::shared_ptr<Mio::UIManager>>::decode(element, uiManager)) {
                    return false;
                }
                rhs.sManager.push_back(uiManager);
            }
            rhs.sUUID = node["uid"].as<Mio::UUid>();
            return true;
        }
    };
}
#endif //GUIMANIFEST_H
