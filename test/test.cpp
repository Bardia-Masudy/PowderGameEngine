#include <gtest/gtest.h>

TEST(CellTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

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

// Test that calculations for Cell collisions provide legal and correct values.
TEST(CellTest, CellCollisionMath) {

}

// Test that orientToVector correctly performs magnitude addition, with frame randomness.
TEST(CellTest, MagnitudeAddition) {

}

// Test that Cells will clamp their speeds to MAX_VELOCITY
TEST(CellTest, RespectMaxVelocity) {

}

// Test that the Chunk successfully simulates Cells in its area.
TEST(ChunkTest, FullChunkCoverage) {

}

// Test that the Chunk doesn't simulate Cells outside its area.
TEST(ChunkTest, NoOutsideCoverage) {

}