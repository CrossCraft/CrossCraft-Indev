#include <Chunk/ChunkStack.hpp>

namespace CrossCraft {
    ChunkStack::ChunkStack(int x, int z) : cX(x), cZ(z) {

        chunks[0] = new ChunkMesh(x, 0, z);
        chunks[1] = new ChunkMesh(x, 1, z);
        chunks[2] = new ChunkMesh(x, 2, z);
        chunks[3] = new ChunkMesh(x, 3, z);
    }

    ChunkStack::~ChunkStack() = default;

    void ChunkStack::prepare() {
        for (auto chunk: chunks) {
            chunk->prepare_mesh();
        }
    }

    void ChunkStack::generate() {
        for (auto chunk: chunks) {
            chunk->generate_mesh();
        }
    }

    void ChunkStack::finalize() {
        for (auto chunk: chunks) {
            chunk->finalize_mesh();
        }
    }

    void ChunkStack::draw(ChunkMeshSelection selection) {
        for (auto chunk: chunks) {
            chunk->draw(selection);
        }
    }
}