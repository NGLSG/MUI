#include "Window.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "Event.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"


#if defined(IMGUI_IMPL_OPENGL_ES3)
#include <SDL3/SDL_opengles3.h>
#else
#include <SDL3/SDL_opengl.h>
#endif


namespace Mio {
    PlatformWindow::PlatformWindow(const char* title, int width, int height, Uint32 flags) {
        /*#if defined(IMGUI_IMPL_OPENGL_ES3)
                cGLSLVersion = "#version 310";
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        #elif defined(__APPLE__)
                cGLSLVersion = "#version 440";
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        #else
                cGLSLVersion = "#version 410";
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        #endif

                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
                SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);*/

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            printf("Error: SDL_Init(): %s\n", SDL_GetError());
            return;
        }

        // Create window with SDL_Renderer graphics context
        pWindow = SDL_CreateWindow(title, width, height, flags);
        if (pWindow == nullptr) {
            printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
            return;
        }
        pRenderer = SDL_CreateRenderer(pWindow, nullptr);
        SDL_SetRenderVSync(pRenderer, 1);
        if (pRenderer == nullptr) {
            SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
            return;
        }
        SDL_SetWindowPosition(pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(pWindow);
    }

    void PlatformWindow::Update() {
        SDL_Event cEvent{};
        while (SDL_PollEvent(&cEvent)) {
            ImGui_ImplSDL3_ProcessEvent(&cEvent);
            if (cEvent.type == SDL_EVENT_QUIT) {
                std::cout << "SDL_EVENT_WINDOW_CLOSE_REQUESTED received." << std::endl;
                Running = false;
            }
            if (cEvent.type == SDL_EVENT_KEY_DOWN) {
                if (cEvent.key.key == SDLK_ESCAPE) {
                }
                std::cout << "Key down: " << cEvent.key.key << std::endl;
            }
            if (cEvent.type == SDL_EVENT_KEY_UP) {
                std::cout << "Key up: " << cEvent.key.key << std::endl;
            }

            if (cEvent.type == SDL_EVENT_WINDOW_RESIZED) {
                std::cout << "Window resized: " << cEvent.window.data1 << "x" << cEvent.window.data2 << std::endl;
                SDL_SetWindowSize(pWindow, cEvent.window.data1, cEvent.window.data2);
            }
        }
        if (SDL_GetWindowFlags(pWindow) & SDL_WINDOW_MINIMIZED) {
            SDL_Delay(10);
        }
    }


    void PlatformWindow::MakeCurrent(SDL_Window* backup_current_window, SDL_GLContext backup_current_context) const {
        if (!SDL_GL_MakeCurrent(pWindow, cGLContext)) {
            std::cerr << SDL_GetError() << std::endl;
        }
    }

    void PlatformWindow::Close() const {
        SDL_DestroyWindow(pWindow);
        //SDL_GL_DestroyContext(cGLContext);
        SDL_DestroyRenderer(pRenderer);
        SDL_Quit();
    }

    void PlatformWindow::SetIcon(const char* path) const {
        if (RC::Utils::File::Exists(path)) {
            SDL_Surface* icon = SDL_LoadBMP(path);
            SDL_SetWindowIcon(pWindow, icon);
            SDL_DestroySurface(icon);
            return;
        }
        std::cerr << "Icon file not found" << std::endl;
    }

    void PlatformWindow::Swap() const {
        SDL_GL_SwapWindow(pWindow);
    }
} // Mio
