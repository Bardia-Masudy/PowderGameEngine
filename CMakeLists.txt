cmake_minimum_required(VERSION 4.2)
project(roguefun)

set(CMAKE_CXX_STANDARD 20)

# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
    # This assumes the SDL_image source is available in vendored/SDL_image
    add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(SDL3_image REQUIRED CONFIG)

endif()

# Create your game executable target as usual
add_executable(roguefun main.cpp
        render/WindowManager.h
        render/WindowManager.cpp
        render/LTexture.cpp
        render/LTexture.h
        simulator/Grid.cpp
        simulator/Grid.h
        materials/Cell.h
        simulator/Chunk.cpp
        simulator/Chunk.h
        materials/Cell.cpp
)
#add_executable(mygame WIN32 mygame.c)

# Link to the actual SDL3 library.
target_link_libraries(roguefun PRIVATE SDL3_image::SDL3_image SDL3::SDL3)