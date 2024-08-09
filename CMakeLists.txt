cmake_minimum_required(VERSION 3.28)
set(name MUI)

file(GLOB src
        *.cpp
        Utils/*.cpp
        Components/*.cpp
)

file(GLOB header
        *.h
        Utils/*.h
        Components/*.h
)
add_subdirectory(Library)

include_directories(Utils Components)
add_library(MUI SHARED STATIC ${src} ${header})
target_link_libraries(MUI PUBLIC RyoCompress ImNodeEditor ImGui glfw EnTT::EnTT yaml-cpp::yaml-cpp)

add_library(Mio::UI ALIAS MUI)


