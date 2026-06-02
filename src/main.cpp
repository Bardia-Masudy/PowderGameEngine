#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "./simulator/Grid.h"
#include "./render/WindowManager.h"
#include "util/GridUtil.h"

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
        Cell::Material material = Cell::WATER;

        SDL_Texture *gridTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ABGR32,
                                                     SDL_TEXTUREACCESS_STREAMING,
                                                     SCREEN_WIDTH, SCREEN_HEIGHT);

        float prevMouseX = -1.f, prevMouseY = -1.f;

        while (quit == false) {
            // Update grid
            grid->step();

            // Get event data
            while (SDL_PollEvent(&e) == true) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                } else if (e.type == SDL_EVENT_KEY_DOWN) {
                    if (e.key.key == SDLK_1) {
                        material = Cell::AIR;
                    } else if (e.key.key == SDLK_2) {
                        material = Cell::WOOD;
                    } else if (e.key.key == SDLK_3) {
                        material = Cell::FIRE;
                    } else if (e.key.key == SDLK_4) {
                        material = Cell::WATER;
                    }
                }
            }

            // Per-frame mouse drawing
            float mouseX, mouseY;
            auto mouseState = SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseState & SDL_BUTTON_LMASK) {
                if (prevMouseX < 0.f || prevMouseY < 0.f) {
                    // First frame of press: single spot
                    grid->setCell(static_cast<int>(mouseX), static_cast<int>(mouseY), material, 5);
                } else {
                    // Held: fill all cells between last and current position
                    // TODO: Known bug, mouse x coordinate wraps off of the edge due to array format.
                    GridUtil::castRay(prevMouseX, prevMouseY, mouseX, mouseY,
                        [&](int cx, int cy) { grid->setCell(cx, cy, material, 5); },
                        [&](int cx, int cy) { return false; },
                        [&](int cx, int cy) {});
                }
                prevMouseX = mouseX;
                prevMouseY = mouseY;
            } else {
                prevMouseX = -1.f;
                prevMouseY = -1.f;
            }

            grid->updateTexture(gridTexture);

            // TODO: Temp particle counting, add to debug options later.
            std::string title = "roguefun | cells: " + std::to_string(grid->countParticles());
            SDL_SetWindowTitle(gWindow, title.c_str());

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
