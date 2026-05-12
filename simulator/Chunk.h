#ifndef ROGUEFUN_CHUNK_H
#define ROGUEFUN_CHUNK_H

class Grid;

// Responsible for stepping a region of Cells on its Grid in a parallelizable manner.
class Chunk {
public:
    Chunk(int x, int y, int width, int height, int offset, Grid *grid);

    Chunk() = delete;

    // If the current frame is appropriate for this Chunk's offset, calls step() on each pixel in it.
    void step() const;

    // Returns if the given coordinates lie within the area visible to Cells in the chunk.
    bool isVisible(int x, int y) const;

private:
    Grid *grid;
    int xMin, yMin, width, height, offset;
};


#endif //ROGUEFUN_CHUNK_H
