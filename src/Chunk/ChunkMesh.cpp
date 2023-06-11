#include <Chunk/ChunkMesh.hpp>
#include <Chunk/FaceConst.hpp>

namespace CrossCraft {

    ChunkMesh::ChunkMesh(int x, int y, int z) : cX(x), cY(y), cZ(z) {
        dirty = true;
    }

    ChunkMesh::~ChunkMesh() = default;

    void ChunkMesh::generate_mesh() {
        // Delete Data
        mesh.transparent.delete_data();
        mesh.opaque.delete_data();
        mesh.opaque.pre_allocate();

        SurroundingPositions surround_pos;
        uint32_t index;
        block_t block;
        mathfu::Vector<float, 3> v;
        auto wd = CC_World_GetData();

        for (size_t y = 0; y < 16; y++) {
            for (size_t z = 0; z < 16; z++) {
                for (size_t x = 0; x < 16; x++) {

                    auto rx = x + cX * 16;
                    auto ry = y + cY * 16;
                    auto rz = z + cZ * 16;

                    index = CC_WIDX(rx, ry, rz, wd);
                    block = wd->blocks[index];

                    if (block == 0) {
                        continue;
                    }

                    surround_pos.set((int) x, (int) y, (int) z);
                    v = mathfu::Vector<float, 3>((float) x, (float) y, (float) z);

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
        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate(
                {static_cast<float>(cX * 16), static_cast<float>(cY * 16), static_cast<float>(cZ * 16)});

        if (dirty) {
            generate_mesh();
        }

        switch (selection) {
            case ChunkMeshSelection::Opaque:
                if (mesh.opaque.idx_counter > 0) {
                    mesh.opaque.mesh.draw();
                }
                break;
            case ChunkMeshSelection::Transparent:
                if (mesh.transparent.idx_counter > 0) {
                    mesh.transparent.mesh.draw();
                }
                break;
        }
    }

    void ChunkMesh::add_block_to_mesh(const WorldData *wd, block_t block, const mathfu::Vector<float, 3> &vector3,
                                      const SurroundingPositions &positions) {

        try_add_face(wd, topFace, block, vector3, positions.up, LIGHT_TOP);
        try_add_face(wd, bottomFace, block, vector3, positions.down, LIGHT_BOT);

        try_add_face(wd, leftFace, block, vector3, positions.left, LIGHT_SIDE_X);
        try_add_face(wd, rightFace, block, vector3, positions.right, LIGHT_SIDE_X);

        try_add_face(wd, frontFace, block, vector3, positions.front, LIGHT_SIDE_Z);
        try_add_face(wd, backFace, block, vector3, positions.back, LIGHT_SIDE_Z);
    }

    int stat = 0;

    void ChunkMesh::try_add_face(const WorldData *wd, const std::array<float, 12> &data, block_t block,
                                 const mathfu::Vector<float, 3> &actual_pos, const mathfu::Vector<int, 3> &check_pos,
                                 uint32_t lightValue) {
        auto check_pos_wx = (int) check_pos.x + cX * 16;
        auto check_pos_wy = (int) check_pos.y + cY * 16;
        auto check_pos_wz = (int) check_pos.z + cZ * 16;

        if (check_pos_wx < 0 || check_pos_wx >= (int) wd->x ||
            check_pos_wy < 0 || check_pos_wy >= (int) wd->y ||
            check_pos_wz < 0 || check_pos_wz >= (int) wd->z) {
            return;
        }

        size_t index = CC_WIDX(check_pos_wx, check_pos_wy, check_pos_wz, wd);
        auto block_check = wd->blocks[index];

        if (block_check == BLK_Air || block_check == BLK_Water || block_check == BLK_Leaves) {
            if (block == BLK_Water && block_check != BLK_Water) {
                std::array<float, 12> data2 = data;
                if (lightValue == LIGHT_TOP) {
                    data2[1] *= 0.9f;
                    data2[4] *= 0.9f;
                    data2[7] *= 0.9f;
                    data2[10] *= 0.9f;

                    stat += 1;
                }
                add_face_to_mesh(data2, getTexCoord(block, lightValue), actual_pos, lightValue, mesh.transparent);
            } else if (block == BLK_Leaves) {
                add_face_to_mesh(data, getTexCoord(block, lightValue), actual_pos, lightValue, mesh.opaque);
            } else {
                if (block != BLK_Water && block != BLK_Glass) {
                    add_face_to_mesh(data, getTexCoord(block, lightValue), actual_pos, lightValue, mesh.opaque);
                }
            }
        }
    }

    void
    ChunkMesh::add_face_to_mesh(const std::array<float, 12> &face, std::array<float, 8> tex,
                                const mathfu::Vector<float, 3> &position,
                                uint32_t value, ChunkMeshInstance &m) {
        Rendering::Color c{};
        c.color = value;

        for (size_t i = 0, tx = 0, idx = 0; i < 4; i++) {
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