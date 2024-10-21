#ifndef MWINDOW_H
#define MWINDOW_H


#include "Library/RyoCompression/utils.h"
#include <SDL3/SDL.h>

#include "Components/Component.h"

namespace Mio {
    class PlatformWindow {
    public:
        PlatformWindow(const char* title, int width, int height, Uint32 flags = 0);

        void Update();

        std::tuple<SDL_Window *, SDL_GLContext> GetData() {
            return std::make_tuple(pWindow, cGLContext);
        }

        SDL_Window* GetWindow() {
            return pWindow;
        }

        const char* GetGLSLVersion() const {
            return cGLSLVersion;
        }

        bool ShouldClose() const {
            return !Running;
        }

        void MakeCurrent(SDL_Window* backup_current_window = nullptr,
                         SDL_GLContext backup_current_context = {}) const;

        void Close() const;

        void SetIcon(const char* path) const;

        void Swap() const;

        SDL_Renderer* GetRenderer() {
            return pRenderer;
        }

        static std::shared_ptr<PlatformWindow> Create(const char* title, int width, int height, Uint32 flags = 0) {
            return std::make_shared<PlatformWindow>(title, width, height, flags);
        }

        SDL_Window* pWindow;
        SDL_GLContext cGLContext;
        SDL_Renderer* pRenderer;

    private:
        bool Running = true;
        const char* cGLSLVersion;
    };
} // Mio

#endif //MWINDOW_H
