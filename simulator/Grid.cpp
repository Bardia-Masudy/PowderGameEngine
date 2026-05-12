#include "Grid.h"

#include <cmath>
#include <vector>
#include <SDL3/SDL_cpuinfo.h>

Grid::Grid(int width, int height) : width{width}, height{height} {
    gridData.reserve(width * height);
    for (int i = 0; i < width * height; i++) {
        gridData.emplace_back(i % width, i / width, this);
    }

    initThreadPool();
}

void Grid::setCell(int x, int y, int material, int radius) {
    for (int xDiff = -radius; xDiff < radius; xDiff++) {
        for (int yDiff = -radius; yDiff < radius; yDiff++) {
            if (xDiff * xDiff + yDiff * yDiff <= radius * radius) {
                int newX = x + xDiff;
                int newY = y + yDiff;
                if (getCell(newX, newY) != nullptr) {
                    Cell *newCell = &gridData.at(newX + newY * width);
                    newCell->setPosition(newX, newY);
                    newCell->setMaterial(Cell::WATER);
                }
            }
        }
    }
}

void Grid::step() {
    for (int i = 0; i < 4; i ++) {
        {
            std::lock_guard lock(poolMutex);
            pendingChunks = static_cast<int>(chunks.size());
            ++frameCount %= 64;
        }
        workReady.notify_all();
        std::unique_lock lock(poolMutex);
        workDone.wait(lock, [this] { return pendingChunks == 0; });
    }
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

void Grid::processThread(int chunkIndex) {
    int lastFrame = 0;
    while (true) {
        std::unique_lock lock(poolMutex);
        workReady.wait(lock, [this, lastFrame] { return stopPool || frameCount != lastFrame; });
        if (stopPool)
            return;
        lastFrame = frameCount;
        lock.unlock();

        chunks[chunkIndex].step();

        lock.lock();
        if (--pendingChunks == 0)
            workDone.notify_one();
    }
}

void Grid::initThreadPool() {
    int numThreads = SDL_GetNumLogicalCPUCores();
    int sideCount = static_cast<int>(std::ceil(std::sqrt(numThreads)));
    int chunkWidth = width / (sideCount);
    int chunkHeight = height / (sideCount);

    for (int y = 0; y < sideCount; y++) {
        for (int x = 0; x < sideCount; x++) {
            int cx = chunkWidth * x;
            int cy = chunkHeight * y;
            int offset = ((x & 1) ? 2 : 0) + ((y & 1) ? 1 : 0);
            chunks.emplace_back(cx, cy, chunkWidth, chunkHeight, offset, this);
            threadPool.emplace_back(&Grid::processThread, this, x + y * sideCount);
        }
    }
}

Grid::~Grid() {
    {
        std::lock_guard lock(poolMutex);
        stopPool = true;
    }
    workReady.notify_all();
    for (auto& t : threadPool)
        t.join();
}

void Grid::updateTexture(SDL_Texture *texture) const {
    void *pixels;
    int pitch;

    if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) == 1) {
        Uint32 *dest = static_cast<Uint32 *>(pixels);

        for (int i = 0; i < width * height; i++)
            dest[i] = gridData.at(i).getColour();

        SDL_UnlockTexture(texture);
    }
}

Cell *Grid::getCell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return nullptr;
    return &gridData.at(x + y * width);
}

int Grid::getWidth() const { return width; }
int Grid::getHeight() const { return height; }
int Grid::getCurrentFrame() const { return frameCount; }