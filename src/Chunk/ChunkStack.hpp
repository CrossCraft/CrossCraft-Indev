#pragma once
#include <Chunk/ChunkUtil.hpp>
#include <Chunk/ChunkMesh.hpp>

namespace CrossCraft{
    using namespace Stardust_Celeste;

    class ChunkStack {
    public:
        ChunkStack(int x, int z);
        ~ChunkStack();

        void draw(ChunkMeshSelection selection);

        std::array<ChunkMesh*, 4> chunks;
    private:
        int cX, cZ;
        ChunkMeshCollection mesh;
    };
}