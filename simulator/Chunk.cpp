#include "Chunk.h"
#include "Grid.h"

Chunk::Chunk(int x, int y, int width, int height, int offset, Grid *grid) : xMin{x}, yMin{y}, width{width},
                                                                            height{height}, grid{grid}, offset{offset} {
}

void Chunk::step() const {
    if (grid->getCurrentFrame() % 4 != offset) { return; }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (Cell *cell = grid->getCell(xMin + x, yMin + y); cell != nullptr && cell->shouldStep()) {
                cell->step(this);
            }
        }
    }
}

bool Chunk::isVisible(int x, int y) const {
    return (x >= xMin - width / 2 &&
            x - xMin < width + width / 2 &&
            y >= yMin - height / 2 &&
            y - yMin < height + height / 2);
}
