
#include "roguefun.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

bool init() {
    // Initialization flag
    bool success{ true };

    // Initialize SDL
    if (SDL_Init( SDL_INIT_VIDEO ) == false ) {
        SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        // Create window
        if ( gWindow = SDL_CreateWindow("SDL3 Testing: Hello!", ScreenWidth, ScreenHeight, 0); gWindow == nullptr) {
            SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Get window surface
            gSurface = SDL_GetWindowSurface(gWindow);
        }
    }

    return success;
}

bool loadMedia() {
    // File loading flag
    bool success{ true };

    // Load splash image
    std::string imagePath{ "../hello.bmp" };
    if (gHelloWorld = SDL_LoadBMP( imagePath.c_str() ); gHelloWorld == nullptr ) {
        SDL_Log("Unable to load image %s ! SDL Error: %s\n", imagePath.c_str(), SDL_GetError() );
        success = false;
    }
    return success;
}

void closeSDL() {
    // Clean up surface
    SDL_DestroySurface( gHelloWorld );
    gHelloWorld = nullptr;

    // Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;
    gSurface = nullptr;

    // Quit SDL subsystems
    SDL_Quit();
}