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
add_executable(MUI ${src} ${header})


