#include <Chunk/ChunkMesh.hpp>
#include <Chunk/FaceConst.hpp>

namespace CrossCraft {

    ChunkMesh::ChunkMesh(int x, int y, int z) : cX(x), cY(y), cZ(z) {
        dirty = true;
    }

    ChunkMesh::~ChunkMesh() {

    }

    void ChunkMesh::generate_mesh() {
        // Delete Data
        mesh.transparent.delete_data();
        mesh.opaque.delete_data();
        mesh.opaque.pre_allocate();

        SurroundingPositions surround_pos;
        uint32_t index;
        block_t block;
        Math::Vector3<int> v;
        auto wd = CC_World_GetData();

        for(size_t y = 0; y < 16; y++) {
            for(size_t z = 0; z < 16; z++) {
                for(size_t x = 0; x < 16; x++) {

                    auto rx = x + cX * 16;
                    auto ry = y + cY * 16;
                    auto rz = z + cZ * 16;

                    index = CC_WIDX(rx, ry, rz, wd);
                    block = wd->blocks[index];

                    if(block == 0) {
                        continue;
                    }

                    surround_pos.set(x + cX, y + cY, z + cZ);
                    v = Math::Vector3<int>(x, y, z);

                    add_block_to_mesh(wd, block, v, surround_pos);
                }
            }
        }

        // Finalize
        mesh.opaque.finalize();
        mesh.transparent.finalize();

        dirty = false;
    }

    void ChunkMesh::draw(CrossCraft::ChunkMeshSelection selection) {
        if(dirty) {
            generate_mesh();
        }

        switch(selection) {
            case ChunkMeshSelection::Opaque:
                mesh.opaque.mesh.draw();
                break;
            case ChunkMeshSelection::Transparent:
                mesh.transparent.mesh.draw();
                break;
        }
    }

    void ChunkMesh::add_block_to_mesh(const WorldData *wd, block_t block, Vector3<int> vector3,
                                      SurroundingPositions positions) {
        try_add_face(wd, topFace, block, vector3, positions.up, LIGHT_TOP);
        try_add_face(wd, bottomFace, block, vector3, positions.down, LIGHT_BOT);

        try_add_face(wd, leftFace, block, vector3, positions.left, LIGHT_SIDE_X);
        try_add_face(wd, rightFace, block, vector3, positions.right, LIGHT_SIDE_X);

        try_add_face(wd, frontFace, block, vector3, positions.front, LIGHT_SIDE_Z);
        try_add_face(wd, backFace, block, vector3, positions.back, LIGHT_SIDE_Z);
    }

    void ChunkMesh::try_add_face(const WorldData *wd, const std::array<float, 12>& data, block_t block, Vector3<int> actual_pos, Vector3<int> check_pos, uint32_t lightValue) {
        auto check_pos_wx = check_pos.x + cX * 16;
        auto check_pos_wy = check_pos.y + cY * 16;
        auto check_pos_wz = check_pos.z + cZ * 16;

        if(check_pos_wx < 0 || check_pos_wx >= (int)wd->x ||
           check_pos_wy < 0 || check_pos_wy >= (int)wd->y ||
           check_pos_wz < 0 || check_pos_wz >= (int)wd->z) {
            return;
        }

        auto index = CC_WIDX(check_pos_wx, check_pos_wy, check_pos_wz, wd);
        auto block_check = wd->blocks[index];

        if(block_check == 0) {
            add_face_to_mesh(data, getTexCoord(block, lightValue), actual_pos, lightValue, mesh.opaque);
        }
    }

    void
    ChunkMesh::add_face_to_mesh(const std::array<float, 12> &face, std::array<float, 8> tex, Vector3<int> position,
                                uint32_t value, ChunkMeshInstance& m) {
        Rendering::Color c;
        c.color = value;

        for(size_t i =0, tx = 0, idx = 0; i < 4; i++) {
            m.mesh.vertices.push_back(Rendering::Vertex{
                tex[tx++],
                tex[tx++],
                c,
                face[idx++] + position.x,
                face[idx++] + position.y,
                face[idx++] + position.z,
            });
        }

        m.mesh.indices.push_back(m.idx_counter + 0);
        m.mesh.indices.push_back(m.idx_counter + 1);
        m.mesh.indices.push_back(m.idx_counter + 2);
        m.mesh.indices.push_back(m.idx_counter + 2);
        m.mesh.indices.push_back(m.idx_counter + 3);
        m.mesh.indices.push_back(m.idx_counter + 0);
        m.idx_counter += 4;
    }
}