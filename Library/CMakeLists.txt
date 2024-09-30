cmake_minimum_required(VERSION 3.22)
find_package(OpenGL REQUIRED)
add_subdirectory(entt)
add_subdirectory(yaml-cpp)
add_subdirectory(RyoCompression)
add_subdirectory(SDL)

file(GLOB isrc
        ImGUI/*.cpp
        ImGUI/backends/imgui_impl_sdl3.*
        ImGUI/backends/imgui_impl_sdlrenderer3.*

        # ImGUI/backends/imgui_impl_glfw.*
        # ImGUI/backends/imgui_impl_opengl3.*
)
set(AndroidLib "")
set(AndroidIncludeDirs "")

# 检查是否为Android平台
if (ANDROID)
    message("Configuring for Android platform")

    # 使用file(GLOB ...)方式收集特定源文件
    file(GLOB isrc
            ImGui/backends/imgui_impl_android.*)
    # 设置Android平台特定的库
    set(AndroidLib
            android
            EGL
            GLESv3
            log)
    # 设置Android平台特定的包含目录
    set(AndroidIncludeDirs "${ANDROID_NDK}/sources/android/native_app_glue")

    # 设置链接器标志，确保ANativeActivity_onCreate符号未被优化掉
    set(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

    # 为目标ImGui添加编译定义
    target_compile_definitions(ImGui PRIVATE
            IMGUI_IMPL_OPENGL_ES3)
endif ()

add_library(ImGui STATIC ${isrc})
target_link_libraries(ImGui PUBLIC SDL3::SDL3 ${OPENGL_gl_LIBRARY})
target_include_directories(ImGui PUBLIC ImGUI)

file(GLOB edSrc
        imgui-node-editor/*.cpp
        imgui-node-editor/*.h
)

add_library(ImNodeEditor STATIC ${edSrc})
target_link_libraries(ImNodeEditor PUBLIC ImGui)
target_include_directories(ImNodeEditor PUBLIC imgui-node-editor)

file(GLOB gSrc
        glad/include/glad/glad.h
        glad/include/KHR/khrplatform.h
        glad/src/glad.c
)

add_library(Glad STATIC ${gSrc})
target_include_directories(Glad PUBLIC glad/include)