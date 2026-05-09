#include "Grid.h"

#include <cmath>
#include <stdexcept>
#include <vector>
#include <SDL3/SDL_cpuinfo.h>

#include "Chunk.h"

Grid::Grid(int width, int height) : width{width}, height{height} {
    gridData.reserve(width * height);
    for (int i = 0; i < width * height; i++) {
        gridData.emplace_back(i % width, i / width, this);
    }
}

void Grid::setCell(int x, int y, int material, int radius) {
    for (int xDiff = -radius; xDiff < radius; xDiff++) {
        for (int yDiff = -radius; yDiff < radius; yDiff++) {
            if (xDiff * xDiff + yDiff * yDiff <= radius * radius) {
                int newX = x + xDiff;
                int newY = y + yDiff;
                if (getCell(newX, newY) != nullptr) {
                    Cell *newCell = &gridData.at(newX + newY * width);
                    newCell->setIndex(newX, newY);
                    newCell->setMaterial(Cell::WATER);
                }
            }
        }
    }
}

void Grid::step() const {
    int numThreads = SDL_GetNumLogicalCPUCores();
    int sideCount = static_cast<int>(std::ceil(std::sqrt(numThreads))) + 1;
    int totalChunks = sideCount * sideCount;

    std::vector<Chunk> chunks;
    std::vector<ThreadData> threadDatas;
    std::vector<SDL_Thread*> threads(totalChunks);

    chunks.reserve(totalChunks);
    threadDatas.reserve(totalChunks);

    for (int t = 0; t < totalChunks; t++) {
        int chunkWidth = width / (sideCount - 1);
        int chunkHeight = height / (sideCount - 1);
        int x = chunkWidth * (t % sideCount);
        int y = chunkHeight * (t / sideCount);

        chunks.emplace_back(x, y, chunkWidth, chunkHeight, this);
        threadDatas.push_back({this, &chunks.back()});
        threads[t] = SDL_CreateThread(getNextState, "simthread", &threadDatas.back());
    }

    for (int t = 0; t < totalChunks; t++)
        SDL_WaitThread(threads[t], nullptr);
    //TODO: check memory leakage for vectors.
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
//                  - Maybe using a "stopped" flag that gets updated by collision/movement or updates nearby
//TODO: Implement fire/wood behaviour, and other generalized particle components.
int Grid::getNextState(void* data) {
    const auto* d = static_cast<ThreadData*>(data);
    d->chunk->step();
    return 0;
}

void Grid::updateTexture(SDL_Texture *texture) const {
    void *pixels;
    int pitch;

    // Lock texture to get a direct pointer to write to
    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) == 1) {
        Uint32 *dest = static_cast<Uint32 *>(pixels);

        for (int i = 0; i < width * height; i++)
            dest[i] = gridData.at(i).getColour();

        SDL_UnlockTexture(texture);
    }
}

Cell *Grid::getCell(int x, int y) const {
    try {
        return const_cast<Cell*>(&gridData.at(x + y * width));
    } catch (std::out_of_range& e) {
        return nullptr;
    }
}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }