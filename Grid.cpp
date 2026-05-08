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
                    gridData[newX + newY * width].mat = state;
            }
        }
    }
}

Grid* Grid::nextGrid() const {
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

//TODO: Move to per-cell behaviour checking (using alternating-regioned threads) to fix current race conditions.
//          - Base simulation on current grid and update in place.
//          - Publicize Grid Cell defn, and seperate Cell logic from Grid. Make grid resp. for simulation calls only.
//          - Each frame, split sim space into a grid of chunks of predetermined size.
//          - Alternate offset of chunking so that each frame's chunks are centered on the corner intersection of prev.
//          - When simulating a chunk, only allow interactions and movement within that chunk.
//              - Chunks can be run in parallel now, but within each chunk particles can have seeded stochastic behavr.
//TODO: Add generalized velocity instead of hardcoded gravity swapping/spreading.
//          - Each cell has v. and h. velocity, calculate movement through fast marching.
//          - Convert excess velocity into (seeded random) "splashing" velocity, for incident and impacted particle,
//              based on density value/ratio.
//          - Implement gravity as global downwards force for applicable particles
//              - Add in static friction to prevent constant "sliding" from prev. splashing velocity.
//TODO: Implement fire/wood behaviour, and other generalized particle components.
int Grid::getNextState(void* data) {
    auto* d = static_cast<ThreadData*>(data);
    for (int i = d->startIndex; i < d->endIndex; i++) {
        char state = AIR;
        switch (d->grid->getStateSafely(i)) {
            case AIR: {
                int w = d->grid->getWidth();
                // Water fell from above
                if (d->grid->getStateSafely(i - w) == WATER)
                    state = WATER;
                // Water spread from right: right neighbor is water that can't fall
                else if (d->grid->getStateSafely(i + 1 - w) == WATER &&
                         d->grid->getStateSafely(i + 1) != AIR &&
                         i % 2 == 0)
                    state = WATER;
                // Water spread from left: left neighbor is water that can't fall
                else if (d->grid->getStateSafely(i - 1 - w) == WATER &&
                         d->grid->getStateSafely(i - 1) != AIR &&
                         i % 2 == 1)
                    state = WATER;
                else
                    state = AIR;
                break;
            }
            case WOOD:
                state = WOOD;
                break;
            case FIRE:
                state = FIRE;
                break;
            case WATER: {
                int w = d->grid->getWidth();
                // Falls straight down
                if (d->grid->getStateSafely(i + w) == AIR)
                    state = AIR;
                // Spreads left
                else if (d->grid->getStateSafely(i - 1 + w) == AIR)
                    state = AIR;
                // Spreads right
                else if (d->grid->getStateSafely(i + 1 + w) == AIR)
                    state = AIR;
                else
                    state = WATER;
                break;
            }
            default:
                state = AIR;
                break;
        }
        d->nextGrid->gridData[i].mat = state;
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
            char s = gridData[i].mat;
            if (s == WOOD)          dest[i] = 0x663300FF;
            else if (s == FIRE)     dest[i] = 0xff0000FF;
            else if (s == WATER)    dest[i] = 0x33ccffFF;
            else                    dest[i] = 0xFFFFFFFF;
        }

        SDL_UnlockTexture(texture);
    }
}

char Grid::getStateSafely( int index ) const {
    if (index > 0 && index < width * height)
        return gridData[index].mat;
    return BORDER;
}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }