#include <iostream>
#include <memory>

#include "Grid.h"
#include "WindowManager.h"

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

        auto currGrid = new Grid(SCREEN_WIDTH, SCREEN_HEIGHT);
        char material = 1;
        bool isMouseDown = false;

        SDL_Texture *gridTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ABGR32,
                                                     SDL_TEXTUREACCESS_STREAMING,
                                                     SCREEN_WIDTH, SCREEN_HEIGHT);

        while (quit == false) {
            // Update grid
            currGrid = currGrid->nextGrid();

            // Get event data
            while (SDL_PollEvent(&e) == true) {
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
                    // TODO: Known bug, mouse x coordinate wraps off of the edge.
                } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || (e.type == SDL_EVENT_MOUSE_MOTION && isMouseDown)) {
                    isMouseDown = true;
                    float mouseX = -1.f, mouseY = -1.f;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    currGrid->setCell(static_cast<int>(mouseX), static_cast<int>(mouseY), material, 4);
                } else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    isMouseDown = false;
                }
            }

            currGrid->updateTexture(gridTexture);

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
