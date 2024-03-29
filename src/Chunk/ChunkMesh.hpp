#pragma once

#include <Chunk/ChunkUtil.hpp>

namespace CrossCraft
{
using namespace Stardust_Celeste;

    extern mathfu::Vector<int, 3> preferredChunkPos;
	void reset_chunk_frame();

class ChunkMesh {
    public:
	ChunkMesh(int x, int y, int z);

	~ChunkMesh();

	void prepare_mesh();
	void generate_mesh();
	void finalize_mesh();

	void draw(ChunkMeshSelection selection);

	bool dirty = false;
	int cX, cY, cZ;
    private:
	ChunkMeshCollection mesh;

	void add_block_to_mesh(const WorldData *wd, block_t block,
			       const mathfu::Vector<int, 3> &vector3,
			       const SurroundingPositions &positions);

	void try_add_face(const WorldData *wd,
			  const std::array<float, 12> &data, block_t block,
			  const mathfu::Vector<int, 3> &actual_pos,
			  const mathfu::Vector<int, 3> &check_pos,
			  uint32_t lightValue);

	void add_face_to_mesh(const std::array<float, 12> &array1,
				     std::array<float, 8> array2,
				     const mathfu::Vector<int, 3> &vector3,
				     uint32_t value, uint32_t value2,
				     ChunkMeshInstance &instance);
	u32 calculateLighting(const WorldData *wd, uint32_t lightValue,
			      size_t index) const;
};
}