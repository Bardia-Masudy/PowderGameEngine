#include "Grid.h"

#include <cstdlib>
#include <stdlib.h>

Grid::Grid( int width, int height ): width{width}, height {height}, gridData{new Pixel[width * height]} {}

Grid::~Grid() { delete [] gridData; } // TODO: Revisit to make sure deletes properly.

void Grid::addPixel( int x, int y, char state) {
    gridData[x + y * width].state = state;
}

Grid* Grid::nextGrid() {
    return this;
    // TODO: implement pixel rules
}

void Grid::updateTexture( SDL_Texture* texture ) const {
    void* pixels;
    int pitch;

    // Lock texture to get a direct pointer to write to
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) == 1) {
        Uint32* dest = static_cast<Uint32*>(pixels);

        for (int i = 0; i < width * height; i++) {
            // Map state to colors (RGBA8888)
            char s = gridData[i].state;
            if (s == 1)      dest[i] = 0x663300FF; // Wood
            else if (s == 2) dest[i] = 0xff0000FF; // Fire
            else if (s == 3) dest[i] = 0x33ccffFF; // Water
            else             dest[i] = 0xFFFFFFFF; // White / Background
        }

        SDL_UnlockTexture(texture);
    }
}
