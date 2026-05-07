#ifndef ROGUEFUN_GRID_H
#define ROGUEFUN_GRID_H
#include <SDL3/SDL_render.h>

// Grid of "pixels" of an arbitrary size. Responsible for storing the pixel state of the game, iterating to new grids
// and capable of being rendered.
class Grid {
public:
    Grid() = delete;

    ~Grid();

    Grid(const Grid &) = delete;

    Grid(Grid &&) = delete;

    // Generate new empty Grid with given dimensions.
    Grid(int width, int height);

    // Add pixel at location.
    void setCell(int x, int y, char state, int radius) const;

    // Given state of pixel and its neighbours,
    static int getNextState(void* target) ;

    // Generates a new grid following established rules for each pixel and deletes this one.
    Grid *nextGrid() const;

    // Converts Grid to SDLSurface of given dimensions.
    void updateTexture(SDL_Texture *) const;

    int getWidth() const;
    int getHeight() const;

    // Returns the state at a position, or -1 if it's outside of bounds.
    char getStateSafely(int index) const;

private:
    // Cell def, stores bitwise info regarding its state.
    typedef struct Cell {
        char state = 0;
    } Cell;

    typedef struct ThreadData {
        int startIndex;
        int endIndex;
        const Grid* grid;
        Grid* nextGrid;
    } ThreadData;

    int width, height;

    // 1D array of cells
    Cell *gridData;
};


#endif //ROGUEFUN_GRID_H
