### Preliminary Test Considerations:
- Test Cells don't go above MAX_VELOCITY
- Test Cells aren't stepped multiple times after one global frame
  - Both within and between two chunks.
- Test collision density and movement calculations are correct
  - Float/int operations are correct
- Test magnitude addition for orientToVector
- Test chunk coverage
  - No doubling of Cells, full coverage of the simulation, and no simulation past borders
- Test for cases with two particles moving towards one location
- Test that wrong-offset threads are skipped


### Implementation Changes:
- Split texture by Chunk and join them on the main thread.
  - Make changed Cells add to a queue for rendering updates rather than calling over every Cell in a Chunk
- Make rendering run every global frame (4 simulation frames) instead of every sim. frame