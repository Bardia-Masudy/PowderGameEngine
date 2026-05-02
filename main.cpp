#include <iostream>
#include "WindowManager.h"

int main( int argc, char* args[] ) {
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

            LTexture* currTexture = &gUpTexture;
            SDL_Color bgColour{ 0xFF, 0xFF, 0xFF, 0xFF };

            while (quit == false) {
                            // Get event data
            while ( SDL_PollEvent( &e ) == true ) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                } else if (e.type == SDL_EVENT_KEY_DOWN) {
                    if (e.key.key == SDLK_UP) {
                        currTexture = &gUpTexture;
                    } else if (e.key.key == SDLK_DOWN) {
                        currTexture = &gDownTexture;
                    } else if (e.key.key == SDLK_LEFT) {
                        currTexture = &gLeftTexture;
                    } else if (e.key.key == SDLK_RIGHT) {
                        currTexture = &gRightTexture;
                    }
                } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    currTexture = &gUpTexture;
                }
            }

            // Reset background colour to white
            bgColour.r = 0xFF;
            bgColour.g = 0xFF;
            bgColour.b = 0xFF;

            //Set background colour based on key state
            const bool* keyStates = SDL_GetKeyboardState( nullptr );
            if( keyStates[ SDL_SCANCODE_UP ] == true )
            {
                //Red
                bgColour.r = 0xFF;
                bgColour.g = 0x00;
                bgColour.b = 0x00;
            }
            else if( keyStates[ SDL_SCANCODE_DOWN ] == true )
            {
                //Green
                bgColour.r = 0x00;
                bgColour.g = 0xFF;
                bgColour.b = 0x00;
            }
            else if( keyStates[ SDL_SCANCODE_LEFT ] == true )
            {
                //Yellow
                bgColour.r = 0xFF;
                bgColour.g = 0xFF;
                bgColour.b = 0x00;
            }
            else if( keyStates[ SDL_SCANCODE_RIGHT ] == true )
            {
                //Blue
                bgColour.r = 0x00;
                bgColour.g = 0x00;
                bgColour.b = 0xFF;
            }

            // Fill the background
            SDL_SetRenderDrawColor( gRenderer, bgColour.r, bgColour.g, bgColour.b, 0xFF );
            SDL_RenderClear( gRenderer );

            // Render image on screen
            currTexture->render((SCREEN_WIDTH - static_cast<float>(currTexture->getWidth())) * 0.5f,
                                (SCREEN_HEIGHT - static_cast<float>(currTexture->getHeight())) * 0.5f);

            // Render mouse "cursor"
            float x = -1.f, y = -1.f;
            SDL_GetMouseState( &x, &y );
            gCursorTexture.render( x, y );

            // Update screen
            SDL_RenderPresent( gRenderer );
            }
        }
    }

    // Clean up
    closeSDL();

    return exitCode;
}