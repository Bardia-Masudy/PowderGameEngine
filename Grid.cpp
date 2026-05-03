#include "Grid.h"

#include <cstdlib>
#include <stdlib.h>

Grid::Grid( int width, int height ): width{width}, height {height}, gridData{new Cell[width * height]} {}

Grid::~Grid() { delete [] gridData; } // TODO: valgrind check

void Grid::setCell( int x, int y, char state, int radius ) const {
    for (int xDiff = -radius; xDiff < radius; xDiff++) {
        for (int yDiff = -radius; yDiff < radius; yDiff++) {
            if (xDiff * xDiff + yDiff * yDiff <= radius * radius) {
                int newX = x + xDiff;
                int newY = y + yDiff;
                if (getStateSafely( newX, newY ) != -1)
                    gridData[newX + newY * width].state = state;
            }
        }
    }
}

Grid* Grid::nextGrid() { // TODO: implement pixel rules.
    auto nextGrid = new Grid(width, height);

    for (int i = 0; i < width * height; i++) {
        if (getStateSafely(i - width) == 3 && gridData[i].state == 0) {
            nextGrid->gridData[i].state = 3;
        } else if (getStateSafely(i + width) == 0 && gridData[i].state == 3) {
            nextGrid->gridData[i].state = 0;
        } else if (getStateSafely(i + width + 1) == 0 && gridData[i].state == 0) {

        } else {
            nextGrid->gridData[i].state = gridData[i].state;
        }
    }

    return nextGrid;
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

char Grid::getStateSafely( int x, int y ) const {
    if (int i = x + y * width; i > 0 && i < width * height)
        return gridData[i].state;
    return -1;
}

char Grid::getStateSafely( int index ) const {
    return getStateSafely( index % width, index / width );
}