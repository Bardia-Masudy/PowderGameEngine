#include <gtest/gtest.h>
#include "materials/Cell.h"
#include "simulator/Grid.h"

// Test that no Cell moving within a Chunk is simulated more than once per global frame.
TEST(GridTest, NoDoubleStepInChunk) {

}

// Test that no Chunk's area lies outside the Grid.
TEST(GridTest, NoInvalidChunkCoverage) {

}

// Test that no two Chunk areas overlap.
TEST(GridTest, NoOverlappingChunkCoverage) {

}

// Test that no Cell moving between Chunks is simulated more than once per frame.
TEST(GridTest, NoDoubleStepBetweenChunks) {

}

// Test that wrong-offset threads are skipped properly.
TEST(GridTest, ThreadOffsetsRespected) {

}

// Test that each global frame runs four simulator frames.
TEST(GridTest, CountSimulatorFrames) {

}

// Test that setCell correctly sets Cells to a fresh instance of the appropriate material.
TEST(GridTest, SetCellsCorrectly) {

}

// Test that the Chunk successfully simulates Cells in its area.
TEST(ChunkTest, FullChunkCoverage) {

}

// Test that the Chunk doesn't simulate Cells outside its area.
TEST(ChunkTest, NoOutsideCoverage) {

}

// Test that a Chunk will return the correct range of visible positions (within and in range).
TEST(ChunkTest, VisibilityIsCorrect) {

}

// TODO: Check these numbers are good final values.
// Test that calculations for Cell collisions provide legal and correct values.
TEST(CellTest, CellCollisionMath) {
    Grid grid(10, 10);

    // a is above b (different y, same x) so only the vertical branch fires.
    Cell *a = grid.getCell(3, 3);
    Cell *b = grid.getCell(3, 4);

    a->setMaterial(Cell::WATER); // density = 2
    a->setVSpeed(10.f);
    a->setHSpeed(2.f);  // positive and non-zero: orientToVector result is stable regardless of rand

    b->setMaterial(Cell::WOOD);  // density = 16
    b->setVSpeed(3.f);           // positive: orientToVector result is stable regardless of rand
    b->setHSpeed(0.f);

    a->collideCells(b);

    // ratio = density_a / (density_a + density_b) = 2 / 18 = 1/9
    // momentum = ratio * vSpeed_a = 10/9
    const float ratio    = 2.f / (2.f + 16.f);
    const float momentum = ratio * 10.f;
    const float eps      = 1e-4f;

    // a transfers its vSpeed: it retains none
    EXPECT_NEAR(a->getVSpeed(), 0.f, eps);

    // b absorbs a's residual vSpeed component (positive direction, since b->vSpeed was positive)
    EXPECT_NEAR(b->getVSpeed(), 3.f + (10.f - momentum), eps);

    // a's hSpeed grows by momentum's magnitude (positive direction, since a->hSpeed was positive)
    EXPECT_NEAR(a->getHSpeed(), 2.f + momentum, eps);

    // b's hSpeed is unaffected (horizontal branch requires x != other->x, but both share x=3)
    EXPECT_NEAR(b->getHSpeed(), 0.f, eps);
}

// Test that orientToVector correctly performs magnitude addition, with frame randomness.
TEST(CellTest, MagnitudeAddition) {
    // Positive vector: magnitude added in positive direction regardless of rand
    EXPECT_EQ(Cell::orientToVector(3.0f,  1.0f, false),  3);
    EXPECT_EQ(Cell::orientToVector(3.0f,  1.0f, true),   3);

    // Negative vector: magnitude added in negative direction regardless of rand
    EXPECT_EQ(Cell::orientToVector(3.0f, -1.0f, false), -3);
    EXPECT_EQ(Cell::orientToVector(3.0f, -1.0f, true),  -3);

    // Zero vector: rand flag breaks a tie for vector = 0.f
    EXPECT_EQ(Cell::orientToVector(4.0f, 0.0f, false),  4);   // >= 0 is true
    EXPECT_EQ(Cell::orientToVector(4.0f, 0.0f, true),  -4);   // > 0 is false
}

// Test that Cells will clamp their speeds to MAX_VELOCITY
TEST(CellTest, RespectMaxVelocity) {
    // Large grid so the cell can fall freely; start mid-column so
    // chunk visibility won't truncate movement before MAX_VELOCITY.
    Grid grid(50, 50);

    const int startRow = 5;
    Cell *cell = grid.getCell(25, startRow);
    cell->setMaterial(Cell::WATER);
    // Inject a speed 3× over the limit; gravity will add a tiny bit more.
    cell->setVSpeed(Cell::MAX_VELOCITY * 3.f);

    Uint32 waterColour = cell->getColour();
    grid.step();

    // Locate the water cell in column 25 after the step.
    int finalRow = -1;
    for (int y = 0; y < 50; y++) {
        if (grid.getCell(25, y)->getColour() == waterColour) {
            finalRow = y;
            break;
        }
    }

    ASSERT_GE(finalRow, 0) << "Water cell not found after step";
    // Clamped: moves at most MAX_VELOCITY rows.
    // Unclamped: would try ~32 rows, landing far past MAX_VELOCITY.
    EXPECT_LE(finalRow - startRow, static_cast<int>(Cell::MAX_VELOCITY));
}

// Test that Cells properly swap in place, and update position to match.
TEST(CellTest, SwapCellsCorrectly) {
    Grid grid(10, 10);

    Cell *a = grid.getCell(3, 3);
    Cell *b = grid.getCell(3, 5);
    a->setMaterial(Cell::WATER);
    b->setMaterial(Cell::FIRE);

    Uint32 colourA = a->getColour();
    Uint32 colourB = b->getColour();

    // Swap a into b's position
    a->swapCells(3, 5);

    // Grid slots must reflect the swap
    EXPECT_EQ(grid.getCell(3, 5)->getColour(), colourA);
    EXPECT_EQ(grid.getCell(3, 3)->getColour(), colourB);
}

// Test that setting a Cell's material correctly updates the relevant fields.
TEST(CellTest, setMaterialWorks) {
    Grid grid(10, 10);
    Cell *cell = grid.getCell(5, 5);

    cell->setMaterial(Cell::AIR);
    EXPECT_EQ(cell->getColour(), 0xffffffFFu);
    EXPECT_FALSE(cell->shouldStep()); // gravity == 0

    cell->setMaterial(Cell::WATER);
    EXPECT_EQ(cell->getColour(), 0x4782d6FFu);
    EXPECT_TRUE(cell->shouldStep());  // gravity == 1

    cell->setMaterial(Cell::FIRE);
    EXPECT_EQ(cell->getColour(), 0xdb300dFFu);
    EXPECT_TRUE(cell->shouldStep());  // gravity == -1

    cell->setMaterial(Cell::WOOD);
    EXPECT_EQ(cell->getColour(), 0x703c1eFFu);
    EXPECT_FALSE(cell->shouldStep()); // gravity == 0
}