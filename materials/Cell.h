#ifndef ROGUEFUN_CELL_H
#define ROGUEFUN_CELL_H
#include <SDL3/SDL_stdinc.h>

#include "../simulator/Chunk.h"

class Cell {
public:
    Cell(int x, int y, Grid *grid);

    Cell() = delete;

    void attemptMove(int hDist, int vDist, const Chunk *chunk);

    // Step cell on its Grid, passing in its Chunk's validation function.
    void step(const Chunk *chunk);

    // Called when calling cell would collide with another due to movement.
    // Perform any displacement depending on density, and other cell-specific behaviour.
    void collideCells(Cell *other);

    void swapCells(int newX, int newY);

    void setPosition(int x, int y);

    Uint32 getColour() const;

    bool shouldStep() const;

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
    int vSpeed, hSpeed;
    Uint32 baseColour;
    int steppedFrame{-1};

    static int orientVector(int added, int comparison, bool flipped);
};


#endif //ROGUEFUN_CELL_H
