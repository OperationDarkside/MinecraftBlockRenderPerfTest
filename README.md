# MinecraftBlockRenderPerfTest

As a performance test for block rendering each tests tries to render 50,000 blocks.

1. 1 draw call / Block  -> 88ms frametime
    - 36 vertices building a block.
    - Before each draw call updates the "model" uniform
2. Like Nr 1 but with Geometry Shader -> 32ms frametime
    - each draw call only sends 8 vertices (4 x 2)
    - the geometry shader creates the block from the 8 vertices
3. Instanced drawing -> 8ms frametime (absolute minimum, less was not possible on my machine)
    - all model-matrices are stored in a separate array buffer
    - each instance call only changes the model matrix and draws the same triangles
