#include <gtest/gtest.h>
#include "materials/Cell.h"

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

// Test that calculations for Cell collisions provide legal and correct values.
TEST(CellTest, CellCollisionMath) {

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

}

// Test that Cells properly swap in place, and update position to match.
TEST(CellTest, SwapCellsCorrectly) {

}

// Test that setting a Cell's material correctly updates the relevant fields.
TEST(CellTest, setMaterialWorks) {

}