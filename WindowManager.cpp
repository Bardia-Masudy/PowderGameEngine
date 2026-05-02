
#include "WindowManager.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

#include "LTexture.h"

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
LTexture gUpTexture, gDownTexture, gLeftTexture, gRightTexture;

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

bool loadMedia() {
    // File loading flag
    bool success{ true };

    //Load directional images
    if( gUpTexture.loadFromFile( "../up.png" ) == false )
    {
        SDL_Log( "Unable to load up image!\n");
        success = false;
    }
    if( gDownTexture.loadFromFile( "../down.png" ) == false )
    {
        SDL_Log( "Unable to load down image!\n");
        success = false;
    }
    if( gLeftTexture.loadFromFile( "../left.png" ) == false )
    {
        SDL_Log( "Unable to load left image!\n");
        success = false;
    }
    if( gRightTexture.loadFromFile( "../right.png" ) == false )
    {
        SDL_Log( "Unable to load right image!\n");
        success = false;
    }

    return success;
}

void closeSDL() {
    // Clean up surface
    gUpTexture.destroy();
    gDownTexture.destroy();
    gLeftTexture.destroy();
    gRightTexture.destroy();

    // Destroy window
    SDL_DestroyRenderer( gRenderer );
    gRenderer = nullptr;
    SDL_DestroyWindow( gWindow );
    gWindow = nullptr;

    // Quit SDL subsystems
    SDL_Quit();
}