#ifndef ROGUEFUN_GRID_H
#define ROGUEFUN_GRID_H
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>

// Grid of "pixels" of an arbitrary size. Responsible for storing the pixel state of the game, iterating to new grids
// and capable of being rendered.
class Grid {
public:
    Grid() = delete;
    ~Grid();
    Grid( const Grid& ) = delete;
    Grid& operator=( const Grid& ) = delete;
    Grid( Grid&& ) = delete;
    Grid& operator=( Grid&& ) = delete;

    // Generate new empty Grid with given dimensions.
    Grid( int width, int height );

    // Add pixel at location.
    void addPixel( int x, int y, char state );

    // Generates a new grid following established rules for each pixel.
    Grid& newGrid( const Grid& oldGrid );

    // Converts Grid to SDLSurface of given dimensions.
    SDL_Surface* getSurface( int width, int height );
private:

    // Pixel def, stores bitwise info regarding its state.
    typedef struct {
        char state;
    } Pixel;

    int width, height;

    // dynamic 1d array pf pixels
    Pixel* gridData;
};


#endif //ROGUEFUN_GRID_H
