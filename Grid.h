#ifndef ROGUEFUN_GRID_H
#define ROGUEFUN_GRID_H
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>

// Grid of "pixels" of an arbitrary size. Responsible for storing the pixel state of the game, iterating to new grids
// and capable of being rendered.
class Grid {
public:
    Grid() = delete;
    ~Grid();
    Grid( const Grid& ) = delete;
    Grid( Grid&& ) = delete;

    // Generate new empty Grid with given dimensions.
    Grid( int width, int height );

    // Add pixel at location.
    void addPixel( int x, int y, char state );

    // Generates a new grid following established rules for each pixel and deletes this one.
    Grid* nextGrid();

    // Converts Grid to SDLSurface of given dimensions.
    void updateTexture(SDL_Texture*) const;

private:

    // Pixel def, stores bitwise info regarding its state.
    typedef struct Pixel {
        char state = 0;
    } Pixel;

    int width, height;

    // dynamic 1d array pf pixels
    Pixel* gridData;
};


#endif //ROGUEFUN_GRID_H
