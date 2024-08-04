cmake_minimum_required(VERSION 3.28)
set(name lib)

find_package(OpenGL REQUIRED)
add_subdirectory(glfw)
add_subdirectory(entt)
add_subdirectory(yaml-cpp)

file(GLOB isrc
        ImGUI/*.cpp
        ImGUI/backends/imgui_impl_glfw.*
        ImGUI/backends/imgui_impl_opengl3.*
        ImGUI/backends/imgui_impl_opengl3_loader.h
)
add_library(ImGui STATIC ${isrc})
target_link_libraries(ImGui PUBLIC glfw ${OPENGL_gl_LIBRARY})
target_include_directories(ImGui PUBLIC ImGUI)

file(GLOB edSrc
        imgui-node-editor/*.cpp
        imgui-node-editor/*.h
)
add_library(ImNodeEditor STATIC ${edSrc})
target_link_libraries(ImNodeEditor PUBLIC ImGui)
target_include_directories(ImNodeEditor PUBLIC imgui-node-editor)