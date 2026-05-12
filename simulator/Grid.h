#ifndef ROGUEFUN_GRID_H
#define ROGUEFUN_GRID_H
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <SDL3/SDL_render.h>

#include "../materials/Cell.h"
#include "Chunk.h"

// Grid of "pixels" of an arbitrary size. Responsible for storing the pixel state of the game, iterating to new grids
// and capable of being rendered.
class Grid {
public:
    Grid() = delete;

    Grid(const Grid &) = delete;

    Grid(Grid &&) = delete;

    // Generate new empty Grid with given dimensions.
    Grid(int width, int height);

    ~Grid();

    // Add pixel at location.
    void setCell(int x, int y, int material, int radius);

    // Advances the simulation by one frame.
    void step();

    // Converts Grid to SDLSurface of given dimensions.
    void updateTexture(SDL_Texture *) const;

    int getCurrentFrame() const;

    // Returns the cell at a position, or nullptr if outside bounds.
    Cell *getCell(int x, int y);

private:
    int width, height;

    // 1D array of cells
    std::vector<Cell> gridData;

    // Pre-built chunk list, computed once in constructor
    std::vector<Chunk> chunks;

    // Thread pool
    std::vector<std::thread> threadPool;
    mutable std::mutex poolMutex;
    mutable std::condition_variable workReady;
    mutable std::condition_variable workDone;
    int pendingChunks{0};
    int frameCount{0};
    bool stopPool{false};

    void processThread(int chunkIndex);

    void initThreadPool();
};


#endif //ROGUEFUN_GRID_H
