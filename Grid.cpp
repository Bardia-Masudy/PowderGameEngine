#include "Grid.h"

Grid::Grid(int width, int height) : width{width}, height{height}, gridData{new Cell[width * height]} {
}

Grid::~Grid() { delete [] gridData; } // TODO: valgrind check

void Grid::setCell(int x, int y, char state, int radius) const {
    for (int xDiff = -radius; xDiff < radius; xDiff++) {
        for (int yDiff = -radius; yDiff < radius; yDiff++) {
            if (xDiff * xDiff + yDiff * yDiff <= radius * radius) {
                int newX = x + xDiff;
                int newY = y + yDiff;
                if (getStateSafely(newX, newY) != -1)
                    gridData[newX + newY * width].state = state;
            }
        }
    }
}

Grid* Grid::nextGrid() const { // TODO: implement pixel rules.
    const auto nextGrid = new Grid(width, height);



    for (int i = width * height - 1; i > 0; i--) {
        char* vicinity[9];
        vicinity[0] = &gridData[i - width - 1].state;
        vicinity[1] = &gridData[i - width    ].state;
        vicinity[2] = &gridData[i - width + 1].state;
        vicinity[3] = &gridData[i         - 1].state;
        vicinity[4] = &gridData[i            ].state;
        vicinity[5] = &gridData[i         + 1].state;
        vicinity[6] = &gridData[i + width - 1].state;
        vicinity[7] = &gridData[i + width    ].state;
        vicinity[8] = &gridData[i + width + 1].state;
        SDL_Thread *thread = SDL_CreateThread(getNextState, "thread", vicinity);

        nextGrid->gridData[i].state = getNextState(vicinity);
    }

    return nextGrid;
}

//(aka int(*)(void *data))

int Grid::getNextState(void* vicinity) {
    switch (vicinity[4]) {
        case 0:
            if (vicinity[1] == 3)
                return 3;
            if (vicinity[0] == 3 && vicinity[3] != 0)
                return 3;
            if (vicinity[2] == 3 && vicinity[5] != 0)
                return 3;
            return 0;
            break;
        case 1: // Wood
            return 1;
            break;
        case 2: // Fire
            return 2;
            break;
        case 3: // Water
            if (vicinity[7] == 0)
                return 0;
            if (vicinity[8] == 0 && vicinity[7] != 0)
                return 0;
            if (vicinity[6] == 0 && vicinity[7] != 0)
                return 0;
            return 3;
            break;
        default:
            return 0;
            break;
    }
}

void Grid::updateTexture(SDL_Texture *texture) const {
    void *pixels;
    int pitch;

    // Lock texture to get a direct pointer to write to
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) == 1) {
        Uint32 *dest = static_cast<Uint32 *>(pixels);

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