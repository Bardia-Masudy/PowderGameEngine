#ifndef ROGUEFUN_CHUNK_H
#define ROGUEFUN_CHUNK_H

class Grid;

// Responsible for stepping a region of Cells on its Grid in a parallelizable manner.
class Chunk {
public:
    Chunk(int x, int y, int width, int height, int offset, Grid* grid);

    Chunk() = delete;

    // Chunk(const Chunk& chunk) = delete;
    //
    // Chunk(Chunk &&) = delete;
    //
    // Chunk& operator=(const Chunk& chunk) = delete;
    //
    // Chunk& operator=(Chunk &&) = delete;

    // Calls step() on each pixel in this Chunk.
    void step() const;

    bool isVisible(int x, int y) const;

private:
    Grid* grid;
    int xMin, yMin, width, height, offset;
};


#endif //ROGUEFUN_CHUNK_H
