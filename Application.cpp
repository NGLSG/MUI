#include "Application.h"
#include <filesystem>
#include <utility>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <backends/imgui_impl_sdl3.h>

#include "GUIManifest.h"
#include "Components/Event.h"
#include "Utils.h"
#include "Variables.h"


namespace Mio {
    void Application::Initialize() {
        /*if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);*/

        //window = glfwCreateWindow(900, 900 / 1.2, name.c_str(), nullptr, nullptr);
        platformWindow = PlatformWindow::Create(name.c_str(), 900, 900 / 1.2,
                                                SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

        /*if (std::filesystem::exists(icon)) {
            GLFWimage image;
            image.pixels = static_cast<unsigned char *>(Utils::LoadTextureToRAM(icon.c_str()));
            glfwSetWindowIcon(window, 1, &image);
        }

        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);*/
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO&io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

        ImGui::StyleColorsLight();


        //ImGui_ImplGlfw_InitForOpenGL(window, true);
        SetStyleDefault();

        //ImGui_ImplOpenGL3_Init(platformWindow->GetGLSLVersion());
        if (!ImGui_ImplSDL3_InitForSDLRenderer(platformWindow->pWindow, platformWindow->pRenderer)) {
            throw std::runtime_error("Failed to initialize ImGui_ImplSDL3_InitForSDLRenderer");
        }
        if (!ImGui_ImplSDLRenderer3_Init(platformWindow->pRenderer)) {
            throw std::runtime_error("Failed to initialize ImGui_ImplSDLRenderer3_Init");
        }

        SetFont(AddFont((ResourcePath / "Assets/font/SourceBlack.otf").string(), 16));
        platformWindow->SetIcon(icon.c_str());
        /*glfwSetErrorCallback(glfw_error_callback);
        glfwSetWindowSizeCallback(window, glfw_window_size_callback);
        glfwSetKeyCallback(window, glfw_key_callback);
        glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);*/
    }

    void Application::Update() const {
        //glfwPollEvents();

        platformWindow->Update();

        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport();
        ImGuiIO&io = ImGui::GetIO();
        //Renderer
        {
            for (auto&ui: CurrentManifest->sManager) {
                ui->Update();
            }
        }

        ImGui::Render();
        /*glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_SetRenderDrawColorFloat(platformWindow->pRenderer, clear_color.x, clear_color.y, clear_color.z,
                                    clear_color.w);
        SDL_RenderClear(platformWindow->pRenderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), platformWindow->pRenderer);
        SDL_RenderPresent(platformWindow->pRenderer);

        /*if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            //GLFWwindow* backup_current_context = glfwGetCurrentContext();
            /*SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();#1#
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            //platformWindow->MakeCurrent(backup_current_window, backup_current_context);
            //glfwMakeContextCurrent(backup_current_context);
        }*/
        //glfwSwapBuffers(window);

        //platformWindow->Swap();
    }

    void Application::Shutdown() const {
        //ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        //ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        platformWindow->Close();
        /*glfwDestroyWindow(window);
        glfwTerminate();*/
    }

    void Application::AddManifest(std::shared_ptr<GUIManifest>&manifest) {
        manifests.emplace_back(manifest);
        SetCurrentManifest(manifest);
    }

    void Application::RemoveManifest(UUid uuid) {
        Mio::erase_if(manifests, [&](const std::shared_ptr<GUIManifest>&item) {
            return item->sUUID == uuid;
        });
        SetCurrentManifest(manifests.back());
    }

    void Application::SetCurrentManifest(std::shared_ptr<GUIManifest>&New) {
        CurrentManifest = New;
    }

    void Application::SetCurrentManifest(UUid uuid) {
        for (auto&manifest: manifests) {
            if (manifest->sUUID == uuid) {
                CurrentManifest = manifest;
                return;
            }
        }
    }

    std::string Application::AddFont(const std::string&fontPath, float size) {
        if (RC::Utils::File::Exists(fontPath)) {
            ImGuiIO&io = ImGui::GetIO();

            const auto font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), size, nullptr,
                                                           io.Fonts->GetGlyphRangesChineseFull());
            std::string name = std::filesystem::path(fontPath).filename().stem().string();
            Fonts[name] = font;
            return name;
        }
        return "";
    }

    void Application::RemoveFont(const std::string&fontPath) {
        Fonts.erase(fontPath);
    }

    void Application::SetFont(const std::string&fontPath) {
        if (Fonts.find(fontPath) == Fonts.end()) {
            return;
        }
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
        style.Colors[ImGuiCol_CheckMark] = ImVec4(0.8f, 0.8f, .8f, 1.00f); // Checkbox checkmark
        style.Colors[ImGuiCol_TabActive] = ImVec4(0.8f, 0.8f, .8f, 1.00f);
        style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
        style.Colors[ImGuiCol_PopupBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
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
