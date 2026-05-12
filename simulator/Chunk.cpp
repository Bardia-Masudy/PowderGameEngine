#include "Chunk.h"
#include "Grid.h"

Chunk::Chunk(int x, int y, int width, int height, Grid *grid) : xMin{x - offset}, yMin{y - offset}, width{width},
                                                                 height{height}, grid{grid}, offset {std::min(width, height)/2} {
}

void Chunk::step() const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (Cell* cell = grid->getCell(xMin + x, yMin + y); cell != nullptr && cell->shouldStep()) {
                cell->step(this);
            }
        }
    }
}

bool Chunk::isVisible(int x, int y) const {
    return (x >= xMin && x - xMin < width && y >= yMin && y - yMin < height);
}

void Chunk::jitter() {
    xMin += offset;
    yMin += offset;
    offset = offset * -1;
}