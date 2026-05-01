#include <iostream>
#include "roguefun.h"

int main() {
    // Final exit code
    int exitCode{ 0 };

    // Initialize
    if ( init() == false ) {
        SDL_Log("Unable to initialize program!\n" );
        exitCode = 1;
    } else {
        // Load media
        if (loadMedia() == false) {
            SDL_Log("Unable to load media!\n" );
            exitCode = 2;
        } else {
            // Quit flag
            bool quit{ false };

            // Event data
            SDL_Event e;
            SDL_zero( e );

            // Main loop
            while ( quit == false ) {
                // Get event data
                while (SDL_PollEvent( &e ) == true ) {
                    if (e.type == SDL_EVENT_QUIT) {
                        quit = true;
                    }
                }

                // Fill the surface white
                SDL_FillSurfaceRect( gSurface, nullptr, SDL_MapSurfaceRGB( gSurface, 0xFF, 0xFF, 0xFF));

                // Render image on screen
                SDL_BlitSurface( gHelloWorld, nullptr, gSurface, nullptr);

                // Update the surface
                SDL_UpdateWindowSurface( gWindow );
            }
        }
    }

    // Clean up
    closeSDL();

    return exitCode;
}