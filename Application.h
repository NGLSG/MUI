#ifndef UI_H
#define UI_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#endif

#include <array>
#include <chrono>
#include <functional>
#include <imgui.h>
#include <utility>

#include "Utils/stb_image.h"
#include <yaml-cpp/node/convert.h>

#include <SDL3/SDL.h>

#include "Window.h"
#include "UIElements.h"
#include "GUIManifest.h"
#include "ResourceManager.h"


namespace Mio {
    class Application {
    public:
        Application() {
        }

        Application(std::string name, std::string iconPath) : name(std::move(name)),
                                                              icon(std::move(iconPath)) {
        }

        void FreeWindowsConsole() {
#ifdef _WIN32
            FreeConsole();
#endif
        }

        void Initialize();

        void Update() const;

        void Shutdown() const;

        void AddManifest(std::shared_ptr<GUIManifest>&manifest);

        void RemoveManifest(UUid uuid);

        void SetCurrentManifest(std::shared_ptr<GUIManifest>&New);

        void SetCurrentManifest(UUid uuid);

        std::string AddFont(const std::string&fontPath, float = 18.f);

        void RemoveFont(const std::string&fontPath);

        void SetFont(const std::string&fontPath);

        void SetClearColor(const ImVec4&color) {
            //ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = color;
            clear_color = color;
        }

        bool ShouldClose() const {
            //return glfwWindowShouldClose(window);
            return platformWindow->ShouldClose();
        }

        std::shared_ptr<PlatformWindow> GetWindow() {
            return platformWindow;
        }


        ~Application() = default;

    private:
        static void SetStyleDefault();

        /*static void glfw_error_callback(int error, const char* description) {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        }

        static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        }

        static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        static void glfw_window_size_callback(GLFWwindow* window, int width, int height) {
            glfwSetWindowSize(window, width, height);
        }*/

        std::string name;
        std::string icon;
        //GLFWwindow* window{};
        std::shared_ptr<PlatformWindow> platformWindow;
        std::shared_ptr<GUIManifest> CurrentManifest;
        bool* show_demo_window = new bool(true);
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        std::vector<std::shared_ptr<GUIManifest>> manifests;
        std::map<std::string, ImFont *> Fonts;
    };

#ifdef _WIN32
#define MAIN_FUNCTION APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
#define MAIN_FUNCTION main(int argc, char** argv)
#endif
}


#endif //UI_H
