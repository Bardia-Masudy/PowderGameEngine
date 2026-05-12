#include <iostream>
#include <memory>

#include "simulator/Grid.h"
#include "render/WindowManager.h"

int main(int argc, char *args[]) {
    // Final exit code
    int exitCode{0};

    // Initialize
    if (init() == false) {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
    } else {
        // Quit flag
        bool quit{false};

        // Event data
        SDL_Event e;
        SDL_zero(e);

        auto grid = new Grid(SCREEN_WIDTH, SCREEN_HEIGHT);
        int material = 0;

        SDL_Texture *gridTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ABGR32,
                                                     SDL_TEXTUREACCESS_STREAMING,
                                                     SCREEN_WIDTH, SCREEN_HEIGHT);

        while (quit == false) {
            // Update grid
            grid->step();

            // Get event data
            while (SDL_PollEvent(&e) == true) {
                // Handle mouse events
                float mouseX = -1.f, mouseY = -1.f;
                SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseState & SDL_BUTTON_LMASK) {
                    grid->setCell(static_cast<int>(mouseX), static_cast<int>(mouseY), material, 5);
                } // TODO: Known bug, mouse x coordinate wraps off of the edge due to array format.


                // Handle keyboard events
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                } else if (e.type == SDL_EVENT_KEY_DOWN) {
                    if (e.key.key == SDLK_1) {
                        material = 0; // White
                    } else if (e.key.key == SDLK_2) {
                        material = 1; // Wood
                    } else if (e.key.key == SDLK_3) {
                        material = 2; // Fire
                    } else if (e.key.key == SDLK_4) {
                        material = 3; // Water
                    }
                }
            }

            grid->updateTexture(gridTexture);

            SDL_RenderClear(gRenderer);
            SDL_RenderTexture(gRenderer, gridTexture, nullptr, nullptr);
            SDL_RenderPresent(gRenderer);
        }
        // Clean up
        SDL_DestroyTexture(gridTexture);
    }

    closeSDL();

    return exitCode;
}
