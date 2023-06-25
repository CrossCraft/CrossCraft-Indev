#pragma once

#include <Chunk/ChunkUtil.hpp>
#include <Chunk/ChunkMesh.hpp>

namespace CrossCraft
{
using namespace Stardust_Celeste;

class ChunkStack {
    public:
	ChunkStack(int x, int z);

	~ChunkStack();

	void prepare();
	void generate();
	void finalize();

	void draw(ChunkMeshSelection selection);

	std::array<ChunkMesh *, 4> chunks{};

    private:
	[[maybe_unused]] int cX, cZ;
	ChunkMeshCollection mesh;
};
}