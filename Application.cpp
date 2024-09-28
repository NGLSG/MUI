#include "Application.h"

#include <filesystem>
#include <utility>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "GUIManifest.h"
#include "Components/Event.h"
#include "Utils.h"
#include "Variables.h"

namespace Mio {
    NodeEditor::NodeEditor(Data data): cData(std::move(data)) {
        config.SettingsFile = cData.savePath.c_str();
        context = ed::CreateEditor(&config);
        ed::SetCurrentEditor(context);
    }

    void NodeEditor::Update() {
        static int counter = 0;
        ed::Begin(cData.name.c_str(), {-1, -1});
        for (auto&node: nodes) {
            node.Update();
        }
        if (ed::ShowBackgroundContextMenu()) {
            ImGui::OpenPopup("NodeContextMenu");
        }
        if (ImGui::BeginPopup("NodeContextMenu")) {
            for (auto&groupNode: Node::Nodes) {
                ImGui::BeginGroup();
                ImGui::Text(groupNode.first.c_str());
                for (auto&node: groupNode.second) {
                    if (ImGui::MenuItem(node.Name.c_str())) {
                        nodes.emplace_back(node);
                    }
                }
                ImGui::EndGroup();
            }
            ImGui::EndPopup();
        }
        if (ed::BeginCreate()) {
            ed::PinId startPinId, endPinId;
            if (ed::QueryNewLink(&startPinId, &endPinId)) {
                if (startPinId && endPinId) {
                    if (ed::AcceptNewItem()) {
                        links.emplace_back(counter++, startPinId, endPinId);
                    }
                }
            }
        }
        ed::EndCreate();

        if (ed::BeginDelete()) {
            ed::LinkId linkId;
            if (ed::QueryDeletedLink(&linkId)) {
                if (ed::AcceptDeletedItem()) {
                    for (auto&link: links) {
                        if (link.ID == linkId) {
                            links.erase(std::remove_if(links.begin(), links.end(), [&](const Link&link) {
                                return link.ID == linkId;
                            }));
                            break;
                        }
                    }
                }
            }
        }
        ed::EndDelete();
        for (auto&link: links) {
            ed::Link(link.ID, link.StartPinID, link.StartPinID, {1, 1, 1, 1}, 2.0f);
        }
        ed::End();
    }

    void NodeEditor::Save(const std::string&path) {
    }

    void NodeEditor::Load(const std::string&path) {
    }

    Button::Button(Data data, std::string name): cData(std::move(data)) {
        uuid = UUid::New();
        AddComponent<Event>();
        cType = Type::Button;
        cName = std::move(name);
    }

    void Button::Modify(const Data&data) {
        cData = data;
        cType = Type::Button;
    }

    void Button::Update() {
        if (Active) {

            UIBase::BeginFrame();
            if (cData.autoSize) {
                ImGui::GetFont()->FontSize = transform.cSize.x / static_cast<float>(converter.from_bytes(cData.name).
                                                 length());
            }
            if (ImGui::Button(cData.name.c_str(), {-1, -1})) {
                GetComponent<Event>().BeginFrame();
            }
            UIBase::EndFrame();
        }
    }

    ImageButton::ImageButton(Data data, std::string name): cData(std::move(data)) {
        textureId = Utils::LoadTexture(cData.imagePath.c_str());
        AddComponent<Event>();
        cType = Type::ImageButton;
        cName = std::move(name);
    }

    void ImageButton::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::ImageButton(reinterpret_cast<void *>(static_cast<intptr_t>(textureId)), {-1, -1}, cData.uv0,
                                   cData.uv1,
                                   cData.frame_padding,
                                   cData.bg_col, cData.tint_col)) {
                GetComponent<Event>().BeginFrame();
            }
            UIBase::EndFrame();
        }
    }

    CheckBox::CheckBox(Data data, std::string name): cData(std::move(data)) {
        AddComponent<Event>();
        cType = Type::CheckBox;
        cName = std::move(name);
        REGISTERVAR(cName.c_str(), cData.value, RefVariables);
    }

    void CheckBox::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::Checkbox(cData.name.c_str(), cData.value.get())) {
                GetComponent<Event>().BeginFrame();
            }
            UIBase::EndFrame();
        }
    }

    bool CheckBox::GetValue() const {
        return *cData.value;
    }

    Text::Text(Data data, std::string name): cData(std::move(data)) {
        cType = Type::Text;
        cName = std::move(name);
    }

    void Text::Update() {
        if (Active) {
            UIBase::BeginFrame();
            auto* draw_list = ImGui::GetWindowDrawList();
            ImVec2 pos = ImGui::GetCursorScreenPos();
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (i == 0 && j == 0)
                        continue;
                    draw_list->AddText(ImGui::GetFont(), cData.text_size.x,
                                       ImVec2(pos.x + i * cData.outline_thickness.x,
                                              pos.y + j * cData.outline_thickness.y),
                                       cData.outline_color, cData.text.c_str(), nullptr);
                }
            }

            // Draw main text
            draw_list->AddText(ImGui::GetFont(), cData.text_size.x, pos, cData.fill_color, cData.text.c_str(), nullptr);
            UIBase::EndFrame();
        }
    }

    InputText::InputText(const Data&data, std::string name): cData(data) {
        AddComponent<Event>();
        cType = Type::InputText;
        cName = std::move(name);
    }

    void InputText::Modify(const Data&data) {
        cData = data;
    }

    std::string InputText::GetValue() const {
        return cData.buf;
    }

    void InputText::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (!cData.Multiline) {
                if (ImGui::InputText(("###" + uuid.toString()).c_str(), cData.buf, cData.buf_size,
                                     cData.flags | ImGuiInputTextFlags_AutoSelectAll,
                                     nullptr,
                                     cData.user_data)) {
                    GetComponent<Event>().BeginFrame();
                }
            }
            else {
                if (ImGui::InputTextMultiline(("###" + uuid.toString()).c_str(), cData.buf, cData.buf_size, {-1, -1},
                                              cData.flags,
                                              nullptr, cData.user_data)) {
                    GetComponent<Event>().BeginFrame();
                }
            }

            UIBase::EndFrame();
        }
    }

    Image::Image(Data data, std::string name): cData(std::move(data)) {
        textureId = Utils::LoadTexture(cData.imagePath.c_str());
        cType = Type::Image;
        cName = std::move(name);
    }

    void Image::Modify(const Data&data) {
        cData = data;
    }

    void Image::Update() {
        if (Active) {
            UIBase::BeginFrame();
            ImGui::Image(reinterpret_cast<void *>(static_cast<intptr_t>(textureId)), {-1, -1}, cData.uv0, cData.uv1,
                         cData.tint_col, cData.border_col);
            UIBase::EndFrame();
        }
    }

    RadioButton::RadioButton(Data data, std::string name): cData(std::move(data)) {
        AddComponent<Event>();
        cType = Type::RadioButton;
        cName = std::move(name);
        REGISTERVAR(cName.c_str(), cData.v, RefVariables);
    }

    void RadioButton::Modify(const Data&data) {
        cData = data;
    }

    void RadioButton::Update() {
        if (Active) {

            UIBase::BeginFrame();
            if (cData.autoSize) {
                ImGui::GetFont()->FontSize = transform.cSize.x / static_cast<float>(converter.from_bytes(cData.name).
                                                 length());
            }
            if (ImGui::RadioButton(cData.name.c_str(), cData.v.get(), cData.v_button)) {
                GetComponent<Event>().BeginFrame();
            }
            UIBase::EndFrame();
        }
    }

    Combo::Combo(Data data, std::string name) : cData(std::move(data)) {
        cType = Type::Combo;
        cName = std::move(name);
    }

    void Combo::Modify(const Data&data) {
        cData = data;
    }

    void Combo::Update() {
        if (Active) {
            UIBase::BeginFrame();

            if (ImGui::BeginCombo(cData.name.c_str(), cData.preview_value, cData.flags)) {
                for (int i = 0; i < cData.items.size(); i++) {
                    const char* item_text = cData.items[i].c_str();
                    if (ImGui::Selectable(item_text, *cData.current_item == i)) {
                        *cData.current_item = i;
                    }
                    if (ImGui::IsItemHovered()) {
                        cData.preview_value = item_text;
                    }
                }
                ImGui::EndCombo();
            }
            UIBase::EndFrame();
        }
    }

    Slider::Slider(Data data, std::string name): cData(std::move(data)) {
        AddComponent<Event>();
        cType = UIBase::Type::Slider;
        cName = std::move(name);
        REGISTERVAR(cName.c_str(), cData.v, RefVariables);
    }

    void Slider::Modify(const Data&data) {
        cData = data;
    }

    void Slider::Update() {
        if (Active) {
            UIBase::BeginFrame();
            switch (cData.type) {
                case Type::Float:
                    if (cData.components == 0) {
                        if (ImGui::SliderScalar(cData.label, ImGuiDataType_Float, cData.v.get(), &cData.v_min,
                                                &cData.v_max,
                                                cData.format, cData.flags)) {
                            GetComponent<Event>().BeginFrame();
                        }
                    }
                    else {
                        if (ImGui::SliderScalarN(cData.label, ImGuiDataType_Float, cData.v.get(), cData.components,
                                                 &cData.v_min,
                                                 &cData.v_max, cData.format, cData.flags)) {
                            GetComponent<Event>().BeginFrame();
                        }
                    }
                    break;
                case Type::Int:
                    if (cData.components == 0) {
                        if (ImGui::SliderScalar(cData.label, ImGuiDataType_S32, cData.v.get(), &cData.v_min,
                                                &cData.v_max,
                                                cData.format, cData.flags)) {
                            GetComponent<Event>().BeginFrame();
                        }
                    }
                    else {
                        if (ImGui::SliderScalarN(cData.label, ImGuiDataType_S32, cData.v.get(), cData.components,
                                                 &cData.v_min,
                                                 &cData.v_max,
                                                 cData.format, cData.flags)) {
                            GetComponent<Event>().BeginFrame();
                        }
                    }
                    break;
                case Type::Angle:
                    if (ImGui::SliderAngle(cData.label, static_cast<float *>(cData.v.get()),
                                           cData.v_min,
                                           cData.v_max, cData.format,
                                           cData.flags)) {
                        GetComponent<Event>().BeginFrame();
                    }
                    break;
                default:
                    break;
            }
            UIBase::EndFrame();
        }
    }

    float Slider::GetValue() const {
        return *std::static_pointer_cast<float>(cData.v);
    }

    ColorPicker::ColorPicker(Data data, std::string name): cData(std::move(data)) {
        cType = Type::ColorPicker;
        cName = std::move(name);
        REGISTERVAR(cName.c_str(), cData.v, RefVariables);
    }

    void ColorPicker::Modify(const Data&data) {
        cData = data;
    }

    void ColorPicker::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::ColorPicker4(cData.name.c_str(), reinterpret_cast<float *>(cData.v.get()), cData.flags,
                                    reinterpret_cast<float *>(&cData.ref_col))) {
                std::cout << "R:" << cData.v->x << " G:" << cData.v->y << " B:" << cData.v->z << " A:" << cData.v->w <<
                        std::endl;
            }
            UIBase::EndFrame();
        }
    }

    ImVec4 ColorPicker::GetValue() const {
        return *cData.v;
    }

    ColorEditor::ColorEditor(Data data, std::string name): cData(std::move(data)) {
        cType = Type::ColorEditor;
        cName = std::move(name);
        REGISTERVAR(cName.c_str(), cData.v, RefVariables);
    }

    void ColorEditor::Modify(const Data&data) {
        cData = data;
    }

    void ColorEditor::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::ColorEdit4(cData.name.c_str(), reinterpret_cast<float *>(cData.v.get()), cData.flags)) {
            }
            UIBase::EndFrame();
        }
    }

    ImVec4 ColorEditor::GetValue() const {
        return *cData.v;
    }

    ProgressBar::ProgressBar(Data data, std::string name): cData(std::move(data)) {
        cType = Type::ProgressBar;
        cName = std::move(name);
        REGISTERVAR(cName.c_str(), cData.progress, RefVariables);
    }

    void ProgressBar::Modify(const Data&data) {
        cData = data;
    }

    void ProgressBar::Update() {
        if (Active) {
            UIBase::BeginFrame();
            ImGui::ProgressBar(*cData.progress, {-1, -1}, cData.overlay);
            UIBase::EndFrame();
        }
    }

    TreeNode::TreeNode(Data data, std::string name): cData(std::move(data)) {
        cType = Type::TreeNode;
        cName = std::move(name);
    }

    void TreeNode::Modify(const Data&data) {
        cData = data;
    }

    void TreeNode::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::TreeNodeEx(cData.name.c_str(), cData.flags)) {
                UIManager::Update();
                ImGui::TreePop();
            }
            UIBase::EndFrame();
        }
    }

    Group::Group(Data data, std::string name): cData((std::move(data))) {
        cName = std::move(name);
        cType = Type::Group;
    }

    void Group::Modify(const Data&data) {
        cData = data;
    }

    void Group::Update() {
        UIBase::BeginFrame();
        ImGui::BeginGroup();
        UIManager::Update();
        ImGui::EndGroup();
        UIBase::EndFrame();
    }

    Console::Console(Data data, std::string name): cData(std::move(data)) {
        cType = Type::Console;
        cName = std::move(name);
    }

    void Console::Modify(const Data&data) {
        cData = data;
    }

    void Console::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::Begin(cData.name.c_str(),NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
                if (ImGui::InputTextWithHint("###Search", "Filter", buf, sizeof(buf),
                                             ImGuiInputTextFlags_EnterReturnsTrue)) {
                    cData.items.clear();
                    for (auto&item: copied) {
                        if (strstr(item.message.c_str(), buf)) {
                            cData.items.push_back(item);
                        }
                    }
                }
                ImGui::Text(cData.name.c_str());
                if (ImGui::BeginChild("###Content", ImVec2(-1, 64), ImGuiChildFlags_None)) {
                    for (auto&item: cData.items) {
                        std::string formattedMessage = "[" + item.time + "] ";
                        switch (item.level) {
                            case LogData::LogDebug:
                                formattedMessage += "DEBUG: ";
                                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), formattedMessage.c_str());
                                break;
                            case LogData::LogTrace:
                                formattedMessage += "TRACE: ";
                                ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), formattedMessage.c_str());
                                break;
                            case LogData::LogInfo:
                                formattedMessage += "INFO: ";
                                ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), formattedMessage.c_str());
                                break;
                            case LogData::LogWarning:
                                formattedMessage += "WARNING: ";
                                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), formattedMessage.c_str());
                                break;
                            case LogData::LogError:
                                formattedMessage += "ERROR: ";
                                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), formattedMessage.c_str());
                                break;
                            case LogData::LogFatal:
                                formattedMessage += "FATAL: ";
                                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), formattedMessage.c_str());
                                break;
                            default:
                                ImGui::Text(formattedMessage.c_str());
                        }
                        ImGui::SameLine();
                        ImGui::TextUnformatted(item.message.c_str());
                    }
                }
                ImGui::EndChild();
                ImGui::End();
            }
            UIBase::EndFrame();
        }
    }

    void Console::AddLog(const LogData&log) {
        cData.items.push_back(log);
        copied.emplace_back(log);
    }

    Popup::Popup(Data data, std::string name): cData(std::move(data)) {
        cType = Type::Popup;
        cName = std::move(name);
    }

    void Popup::Modify(const Data&data) {
        cData = data;
    }

    void Popup::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (open) {
                ImGui::OpenPopup(cData.name.c_str());
            }
            if (ImGui::BeginPopup(cData.name.c_str())) {
                UIManager::Update();
                ImGui::EndPopup();
            }
            UIBase::EndFrame();
        }
    }

    Tooltip::Tooltip(Data data, std::string name): cData(std::move(data)) {
        cType = Type::Tooltip;
        cName = std::move(name);
    }

    void Tooltip::Modify(const Data&data) {
        cData = data;
    }

    void Tooltip::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                BeginFrame();
                ImGui::EndTooltip();
            }
            UIBase::EndFrame();
        }
    }

    ListBox::ListBox(Data data, std::string name) : cData(std::move(data)) {
        cType = Type::ListBox;
        cName = std::move(name);
    }

    void ListBox::Modify(const Data&data) {
        cData = data;
    }

    void ListBox::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::BeginListBox(cData.name.c_str(), {-1, -1})) {
                for (int i = 0; i < cData.items.size(); i++) {
                    if (!cData.items.empty()) {
                        const char* item_text = cData.items[i].c_str();
                        if (ImGui::Selectable(item_text, cData.current_item == i)) {
                            cData.current_item = i;
                        }
                        if (ImGui::IsItemHovered()) {
                            ImGui::SetTooltip(item_text);
                        }
                    }
                }
                ImGui::EndListBox();
            }

            UIBase::EndFrame();
        }
    }

    SubWindow::SubWindow(Data data, std::string name): cData(std::move(data)) {
        cType = Type::SubWindow;
        cName = std::move(name);
    }

    void SubWindow::Modify(const Data&data) {
        cData = data;
    }

    void SubWindow::Update() {
        if (Active) {
            UIBase::BeginFrame();

            ImGui::BeginChild(cData.name.c_str(), cData.size, cData.child_flags, cData.window_flags);
            UIManager::Update();
            ImGui::EndChild();
            UIBase::EndFrame();
        }
    }

    Window::Window(Data data, std::string name): cData(std::move(data)) {
        cType = Type::Window;
        cName = std::move(name);
    }

    void Window::Modify(const Data&data) {
        cData = data;
    }

    void Window::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (cData.fullScreen) {
                ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
                ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);
                ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
            }
            if (ImGui::Begin(cData.name.c_str(), &cData.p_open, cData.flags)) {
                UIManager::Update();
            }
            ImGui::End();
            UIBase::EndFrame();
        }
    }

    MenuItem::MenuItem(Data data, std::string name): cData(std::move(data)) {
        AddComponent<Event>();
        cType = Type::MenuItem;
        cName = std::move(name);
    }

    void MenuItem::Modify(const Data&data) {
        cData = data;
    }

    void MenuItem::Update() {
        UIBase::BeginFrame();
        if (ImGui::MenuItem(cData.name.c_str(), cData.shortcut)) {
            GetComponent<Event>().BeginFrame();
        }
        UIBase::EndFrame();
    }

    MenuBar::MenuBar(Data data, std::string name): cData(std::move(data)) {
        cType = Type::MenuBar;
        cName = std::move(name);
    }

    void MenuBar::Modify(const Data&data) {
        cData = data;
    }

    void MenuBar::Update() {
        if (Active) {
            UIBase::BeginFrame();
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu(cData.name.c_str())) {
                    for (auto&item: menuItems) {
                        item->Update();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            UIBase::EndFrame();
        }
    }

    void MenuBar::AddMenuItem(const std::shared_ptr<MenuItem>&menuItem) {
        menuItems.emplace_back(menuItem);
    }

    void MenuBar::RemoveMenuItem(UUid uuid) {
        Mio::erase_if(menuItems, [&](const std::shared_ptr<MenuItem>&item) {
            return item->UID() == uuid;
        });
    }

    void Application::Initialize() {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        window = glfwCreateWindow(900, 900 / 1.2, name.c_str(), nullptr, nullptr);
        if (std::filesystem::exists(icon)) {
            GLFWimage image;
            image.pixels = static_cast<unsigned char *>(Utils::LoadTextureToRAM(icon.c_str()));
            glfwSetWindowIcon(window, 1, &image);
        }

        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO&io = ImGui::GetIO();
        SetFont(AddFont((ResourcePath / "assets/font/SourceBlack.otf").string(), 16));
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

        SetStyleDefault();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        glfwSetErrorCallback(glfw_error_callback);
        glfwSetWindowSizeCallback(window, glfw_window_size_callback);
        glfwSetKeyCallback(window, glfw_key_callback);
        glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
    }

    void Application::Update() {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
        ImGuiIO&io = ImGui::GetIO(); {
            for (auto&item: manifests) {
                for (auto&ui: item->sManager) {
                    ui->Update();
                }
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui::EndFrame();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(window);
    }

    void Application::Shutdown() const {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Application::AddManifest(std::shared_ptr<GUIManifest>&manifest) {
        manifests.emplace_back(manifest);
    }

    void Application::RemoveManifest(UUid uuid) {
        Mio::erase_if(manifests, [&](const std::shared_ptr<GUIManifest>&item) {
            return item->sUUID == uuid;
        });
    }

    std::string Application::AddFont(const std::string&fontPath, float size) {
        ImGuiIO&io = ImGui::GetIO();

        const auto font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), size, nullptr,
                                                       io.Fonts->GetGlyphRangesChineseFull());
        std::string name = std::filesystem::path(fontPath).filename().stem().string();
        Fonts[name] = font;
        return name;
    }

    void Application::RemoveFont(const std::string&fontPath) {
        Fonts.erase(fontPath);
    }

    void Application::SetFont(const std::string&fontPath) {
        ImGuiIO&io = ImGui::GetIO();
        io.FontDefault = Fonts[fontPath];
    }

    void Application::SetStyleDefault() {
        ImGuiStyle&style = ImGui::GetStyle();
        style.FrameRounding = 4.0f;
        style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f); // Black text
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f); // Light gray background
        style.Colors[ImGuiCol_Button] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f); // Button background
        style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f); // Hover effect
        style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f); // Pressed effect
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.3f, 0.4f, 1.0f); // Inactive window title background color
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 0.35f, 0.45f, 1.0f);
        // Active window title background color
        style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.4f, 0.5f, 1.0f);
        // Collapsed window title background color
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f); // Slider handle
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f); // Pressed slider handle
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Input box background
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Input box hover
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.47f, 1.00f, 1.00f); // Checkbox checkmark
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
        style.Colors[ImGuiCol_PopupBg]=ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        style.ChildBorderSize = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.FrameBorderSize = 1.0f; // Border size
        style.ItemSpacing = ImVec2(8, 4); // Item spacing
        style.WindowBorderSize = 0.0f; // Window border
        style.WindowRounding = 4.0f; // Window rounding
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f); // Title centered
        Style::DefaultStyle = style;
    }
}
