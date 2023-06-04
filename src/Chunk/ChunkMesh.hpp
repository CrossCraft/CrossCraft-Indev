#pragma once
#include <Chunk/ChunkUtil.hpp>

namespace CrossCraft{
    using namespace Stardust_Celeste;

    class ChunkMesh {
    public:
        ChunkMesh(int x, int y, int z);
        ~ChunkMesh();

        void generate_mesh();

        void draw(ChunkMeshSelection selection);

        bool dirty = false;
    private:
        int cX, cY, cZ;

        ChunkMeshCollection mesh;

        void add_block_to_mesh(const WorldData *wd, block_t block, Vector3<int> vector3, SurroundingPositions positions);
        void try_add_face(const WorldData *wd, const std::array<float, 12>& data, block_t block, Vector3<int> actual_pos, Vector3<int> check_pos, uint32_t lightValue);

        void
        add_face_to_mesh(const std::array<float, 12> &array1, std::array<float, 8> array2, Vector3<int> vector3,
                         uint32_t value,
                         ChunkMeshInstance& instance);
    };
}