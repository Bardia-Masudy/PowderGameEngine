#include "Cell.h"
#include "../simulator/Grid.h"

Cell::Cell(int x, int y, Grid *grid) : x{x}, y{y}, grid{grid} {
    vSpeed = 0;
    hSpeed = 0;
    setMaterial(AIR);
}

void Cell::step(const Chunk *chunk) {
    //TODO: do movement and other interactions.
    if (gravity != 0 && chunk->contains(x, y + 1) && grid->getCell(x, y + 1) != nullptr) {
        int myX = x, myY = y;
        Cell* below = grid->getCell(x, y + 1);
        std::swap(*this, *below);
        this->x = myX; this->y = myY;
        below->x = myX; below->y = myY + 1;
    }
}

Uint32 Cell::getColour() const {
    return baseColour;
}

void Cell::setIndex(int x, int y) {
    this->x = x;
    this->y = y;
}

void Cell::setMaterial(int material) {
    switch (material) {
        case AIR:
            density = 0;
            gravity = 0;
            baseColour = 0xffffffFF;
            break;
        case WATER:
            density = 0;
            gravity = 1;
            baseColour = 0x4782d6FF;
            break;
        case FIRE:
            density = 0;
            gravity = -1;
            baseColour = 0xdb300dFF;
            break;
        case WOOD:
            density = 0;
            gravity = 0;
            baseColour = 0x703c1eFF;
            break;
        default:
            density = 0;
            gravity = 0;
            baseColour = 0x8716a1FF;
            break;
    }
}