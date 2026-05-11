#ifndef ROGUEFUN_CELL_H
#define ROGUEFUN_CELL_H
#include <SDL3/SDL_stdinc.h>

#include "../simulator/Chunk.h"

class Cell {
public:
    Cell(int x, int y, Grid *grid);

    Cell() = delete;

    // Step cell on its Grid, passing in its Chunk's validation function.
    void step(const Chunk *chunk);

    Uint32 getColour() const;

    void setIndex(int x, int y);

    void setMaterial(int material);

    enum Material {
        AIR,
        WATER,
        FIRE,
        WOOD
    };

private:
    int x, y;
    Grid *grid;
    char density;
    char gravity;
    // bool isStill;
    int vSpeed;
    int hSpeed;
    Uint32 baseColour;
    int steppedFrame{-1};
};


#endif //ROGUEFUN_CELL_H
