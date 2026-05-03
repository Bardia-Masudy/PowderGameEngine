
#include "WindowManager.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
// LTexture gUpTexture, gDownTexture, gLeftTexture, gRightTexture, gCursorTexture;

bool init() {
    // Initialization flag
    bool success{ true };

    // Initialize SDL
    if (SDL_Init( SDL_INIT_VIDEO ) == false ) {
        SDL_Log("SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    } else {
        // Create window
        if ( SDL_CreateWindowAndRenderer("SDL3 Testing: Hello!", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &gWindow, &gRenderer) == false) {
            SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
    }

    return success;
}

void closeSDL() {
    // Destroy window
    SDL_DestroyRenderer( gRenderer );
    gRenderer = nullptr;
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;

    // Quit SDL subsystems
    SDL_Quit();
}