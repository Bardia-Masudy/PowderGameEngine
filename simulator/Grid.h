#ifndef ROGUEFUN_GRID_H
#define ROGUEFUN_GRID_H
#include <vector>
#include <SDL3/SDL_render.h>

#include "../materials/Cell.h"
class Chunk;

// Grid of "pixels" of an arbitrary size. Responsible for storing the pixel state of the game, iterating to new grids
// and capable of being rendered.
class Grid {
public:
    Grid() = delete;

    Grid(const Grid &) = delete;

    Grid(Grid &&) = delete;

    // Generate new empty Grid with given dimensions.
    Grid(int width, int height);

    // Add pixel at location.
    void setCell(int x, int y, int material, int radius);

    // Given state of pixel and its neighbours,
    static int getNextState(void* target) ;

    // Generates a new grid following established rules for each pixel and deletes this one.
    void step() const;

    // Converts Grid to SDLSurface of given dimensions.
    void updateTexture(SDL_Texture *) const;

    int getWidth() const;
    int getHeight() const;

    // Returns the cell at a position, or -1 if it's outside of bounds.
    Cell *getCell(int x, int y) const;

private:
    typedef struct ThreadData {
        const Grid* grid;
        const Chunk* chunk;
    } ThreadData;

    int width, height;

    // 1D array of cells
    std::vector<Cell> gridData;
};


#endif //ROGUEFUN_GRID_H
