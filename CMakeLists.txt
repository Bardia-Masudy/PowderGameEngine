cmake_minimum_required(VERSION 4.2)
project(roguefun)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

## SDL3
# Create an option to switch between a system sdl library and a vendored SDL library
option(MYGAME_VENDORED "Use vendored libraries" OFF)

if (MYGAME_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
    # This assumes the SDL_image source is available in vendored/SDL_image
    add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)
else ()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
    find_package(SDL3_image REQUIRED CONFIG)
endif ()

# Create your game executable target as usual
add_executable(roguefun
        src/main.cpp
        src/render/WindowManager.h
        src/render/WindowManager.cpp
        src/render/LTexture.cpp
        src/render/LTexture.h
        src/simulator/Grid.cpp
        src/simulator/Grid.h
        src/simulator/Chunk.cpp
        src/simulator/Chunk.h
        src/materials/Cell.cpp
        src/materials/Cell.h
        src/util/GridUtil.cpp
        src/util/GridUtil.h
)

# Link to the actual SDL3 library.
target_link_libraries(roguefun PRIVATE SDL3_image::SDL3_image SDL3::SDL3)

# Enable testing for GTest
enable_testing()

# Test executable
add_executable(roguefun_tests
        test/test.cpp
        src/render/WindowManager.h
        src/render/WindowManager.cpp
        src/render/LTexture.cpp
        src/render/LTexture.h
        src/simulator/Grid.cpp
        src/simulator/Grid.h
        src/simulator/Chunk.cpp
        src/simulator/Chunk.h
        src/materials/Cell.cpp
        src/materials/Cell.h
)
target_include_directories(roguefun_tests PRIVATE src)
target_link_libraries(roguefun_tests PRIVATE GTest::gtest_main SDL3_image::SDL3_image SDL3::SDL3)

include(GoogleTest)
gtest_discover_tests(roguefun_tests)