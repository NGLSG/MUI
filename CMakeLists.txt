cmake_minimum_required(VERSION 3.22)
set(name MUI)
set(CMAKE_CXX_STANDARD 17)
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
add_definitions(-DNOCRYPT)
add_library(MUI SHARED STATIC ${src} ${header})
target_link_libraries(MUI PUBLIC RyoCompress ImNodeEditor ImGui EnTT::EnTT yaml-cpp::yaml-cpp ${AndroidLib} Glad)
if (WIN32)
    target_compile_definitions(MUI PRIVATE -DWIN32_LEAN_AND_MEAN)
    target_compile_definitions(MUI PRIVATE -DWINDOWS)
    set_target_properties(MUI PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()
target_include_directories(MUI PUBLIC ${AndroidIncludeDirs})
add_library(Mio::UI ALIAS MUI)


