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

    // Add a circle of pixels at given location on the Grid, of given material and radius.
    void setCell(int x, int y, Cell::Material material, int radius);

    // Advances the simulation by one global frame.
    void step();

    // Converts Grid to SDLSurface of given dimensions.
    void updateTexture(SDL_Texture *) const;

    // Returns current (simulator) frame.
    int getCurrentFrame() const;

    // Returns the cell at a position, or nullptr if outside bounds.
    Cell *getCell(int x, int y);

private:
    int width, height;

    // 1D array of cells
    std::vector<Cell> gridData;

    // Pre-built chunk list.
    std::vector<Chunk> chunks;

    // Thread pool
    std::vector<std::thread> threadPool;
    mutable std::mutex poolMutex;
    mutable std::condition_variable workReady;
    mutable std::condition_variable workDone;
    int pendingChunks{0};
    int frameCount{0};
    bool stopPool{false};

    // Run the process for a thread. TODO: Skip wrong-offset threads before locking.
    void processThread(int chunkIndex);

    // Initialize the thread pool and chunk vector.
    void initThreadPool();
};


#endif //ROGUEFUN_GRID_H
