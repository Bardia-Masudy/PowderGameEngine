#include "Grid.h"

#include <SDL3/SDL_cpuinfo.h>

Grid::Grid(int width, int height) : width{width}, height{height}, gridData{new Cell[width * height]} {
}

Grid::~Grid() { delete [] gridData; } // TODO: valgrind check

void Grid::setCell(int x, int y, char state, int radius) const {
    for (int xDiff = -radius; xDiff < radius; xDiff++) {
        for (int yDiff = -radius; yDiff < radius; yDiff++) {
            if (xDiff * xDiff + yDiff * yDiff <= radius * radius) {
                int newX = x + xDiff;
                int newY = y + yDiff;
                if (getStateSafely(newX + width * newY) != -1)
                    gridData[newX + newY * width].state = state;
            }
        }
    }
}

Grid* Grid::nextGrid() const { // TODO: implement pixel rules.
    const auto next = new Grid(width, height);
    int numThreads = SDL_GetNumLogicalCPUCores();
    int total = width * height;

    SDL_Thread** threads = new SDL_Thread* [numThreads];

    for (int t = 0; t < numThreads; t++) {
        int start = (total * t) / numThreads;
        int end = (total * (t+1)) / numThreads;
        auto* data = new ThreadData{start, end, this, next};
        threads[t] = SDL_CreateThread(getNextState, "simthread", data);
    }

    for (int t = 0; t < numThreads; t++)
        SDL_WaitThread(threads[t], nullptr);

    delete[] threads;
    return next;
}

int Grid::getNextState(void* data) {
    auto* d = static_cast<ThreadData*>(data);
    for (int i = d->startIndex; i < d->endIndex; i++) {
        switch (d->grid->getStateSafely(i)) {
            case 0:
                d->nextGrid->gridData[i].state = 0;
                break;
            case 1: // Wood
                d->nextGrid->gridData[i].state = 1;
                break;
            case 2: // Fire
                d->nextGrid->gridData[i].state = 2;
                break;
            case 3: // Water
                d->nextGrid->gridData[i].state = 3;
                break;
            default:
                d->nextGrid->gridData[i].state = 0;
                break;
        }
    }
    delete d;
    return 0;
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

char Grid::getStateSafely( int index ) const {
    if (index > 0 && index < width * height)
        return gridData[index].state;
    return -1;
}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }