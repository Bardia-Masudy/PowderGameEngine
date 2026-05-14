#ifndef ROGUEFUN_CELL_H
#define ROGUEFUN_CELL_H
#include <SDL3/SDL_stdinc.h>

#include "../simulator/Chunk.h"

class Cell {
public:
    Cell(int x, int y, Grid *grid);

    Cell() = delete;

    // Attempts to move the pixel along less dense space with given vertical and horizontal speeds.
    // Inspired by Alois Zingl's page "The Beauty of Bresenham's Algorithm".
    void attemptMove(int hDist, int vDist, const Chunk *chunk);

    // Step cell on its Grid, passing in its Chunk's validation function.
    void step(const Chunk *chunk);

    // Called when calling cell would collide with another due to movement.
    // Perform any displacement depending on density, and other cell-specific behaviour.
    void collideCells(Cell *other);

    // Swap two cells on the grid, updating each cell's coordinates accordingly.
    void swapCells(int newX, int newY);

    // Sets the position values of this Cell. Does not override Grid data.
    void setPosition(int x, int y);

    // Return this Cell's colour.
    Uint32 getColour() const;

    // Return whether this Cell should run the step() command.
    bool shouldStep() const;

    // Set the material (and relevant characteristic) of this cell based on given enum.
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
    float vSpeed, hSpeed;
    Uint32 baseColour;
    int steppedFrame{-1};

    // Return vector additively scaled by magnitude. Uses rand to break ties.
    static int orientToVector(float magnitude, float vector, bool rand);
};


#endif //ROGUEFUN_CELL_H
