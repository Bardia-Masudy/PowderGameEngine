#include "Chunk.h"
#include "Grid.h"

Chunk::Chunk(int x, int y, int width, int height, const Grid *grid) : xMin{x}, yMin{y}, width{width},
                                                                      height{height}, grid{grid} {
}

void Chunk::step() const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (Cell* cell = grid->getCell(xMin + x, yMin + y); cell != nullptr)
                cell->step(this);
        }
    }
}

bool Chunk::contains(int x, int y) const {
    return (x >= xMin && x - xMin < width && y >= yMin && y - yMin < height);
}