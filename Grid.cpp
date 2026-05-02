#include "Grid.h"

Grid::Grid( int width, int height ): width{width}, height {height}, gridData{new Pixel[width * height]} {}

Grid::~Grid() { delete [] gridData; } // TODO: Revisit to make sure deletes properly.

void Grid::addPixel( int x, int y, char state) {
    if (gridData[x + y * width].state == 0) {
        gridData[x + y * width].state = state;
    } else {
        // do nothing for now, TODO: implement overrides.
    }
}

Grid& newGrid( const Grid& oldGrid ) {
    // TODO: implement pixel rules
}

SDL_Surface* getSurface( int width, int height ) {
    // TODO: implement rendering
}