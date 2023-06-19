#include <Chunk/ChunkMesh.hpp>
#include <Chunk/FaceConst.hpp>
#include <Chunk/Frustum.hpp>
#include <Chunk/ChunkMeta.hpp>

namespace CrossCraft {

    ChunkMesh::ChunkMesh(int x, int y, int z) : cX(x), cY(y), cZ(z) {
        dirty = true;
    }

    ChunkMesh::~ChunkMesh() = default;

    void ChunkMesh::finalize_mesh() {
        // Finalize
        mesh.opaque.finalize();
        mesh.transparent.finalize();

        dirty = false;
    }

    void ChunkMesh::prepare_mesh() {
        // Delete Data
        mesh.transparent.delete_data();
        mesh.opaque.delete_data();
        mesh.opaque.pre_allocate();
    }

    void ChunkMesh::generate_mesh() {
        SurroundingPositions surround_pos;
        uint32_t index;
        block_t block;
        mathfu::Vector<int, 3> v;
        auto wd = CC_World_GetData();

        // Check if chunk is empty
        if (ChunkMeta::getData(cX, cY, cZ).isEmpty) {
            goto end;
        }
        // Check if chunk is full
        if (ChunkMeta::getData(cX, cY, cZ).isFull) {
            goto end;
        }

        for (int y = 0; y < 16; y++) {
            for (int z = 0; z < 16; z++) {
                for (int x = 0; x < 16; x++) {

                    auto rx = x + cX * 16;
                    auto ry = y + cY * 16;
                    auto rz = z + cZ * 16;

                    index = CC_WIDX(rx, ry, rz, wd);
                    block = wd->blocks[index];

                    if (block == 0) {
                        continue;
                    }

                    surround_pos.set(x, y, z);
                    v = mathfu::Vector<int, 3>(x, y, z);

                    if (block == BLK_Flower1 || block == BLK_Flower2 || block == BLK_Mushroom1 || block == BLK_Mushroom2) {
                        auto texCoord = getTexCoord(block, LIGHT_TOP);
                        std::array<float, 8> texCoord2 = {texCoord[2], texCoord[3], texCoord[4], texCoord[5], texCoord[6], texCoord[7], texCoord[0], texCoord[1]};
                        add_face_to_mesh(xFace1, texCoord, v, LIGHT_TOP, mesh.transparent);
                        add_face_to_mesh(xFace2, texCoord, v, LIGHT_TOP, mesh.transparent);
                        add_face_to_mesh(xFace3, texCoord2, v, LIGHT_TOP, mesh.transparent);
                        add_face_to_mesh(xFace4, texCoord2, v, LIGHT_TOP, mesh.transparent);
                        continue;
                    }

                    if(block == BLK_Slab) {
                        add_face_to_mesh(topFaceHalf, getTexCoord(block, LIGHT_TOP), v, LIGHT_TOP, mesh.opaque);
                        try_add_face(wd, bottomFace, block, v, surround_pos.down, LIGHT_BOT);

                        try_add_face(wd, leftFaceHalf, block, v, surround_pos.left, LIGHT_SIDE_X);
                        try_add_face(wd, rightFaceHalf, block, v, surround_pos.right, LIGHT_SIDE_X);

                        try_add_face(wd, frontFaceHalf, block, v, surround_pos.front, LIGHT_SIDE_Z);
                        try_add_face(wd, backFaceHalf, block, v, surround_pos.back, LIGHT_SIDE_Z);
                        continue;
                    }

                    add_block_to_mesh(wd, block, v, surround_pos);
                }
            }
        }

end:
        return;
    }

    void ChunkMesh::draw(CrossCraft::ChunkMeshSelection selection) {
        if(!Frustum::get().IsBoxVisible(mathfu::Vector<float, 3>(cX * 16, cY * 16, cZ * 16), mathfu::Vector<float, 3>(cX * 16 + 16, cY * 16 + 16, cZ * 16 + 16))) {
            return;
        }

        if (dirty) {
            prepare_mesh();
            generate_mesh();
            finalize_mesh();
        }

        switch (selection) {
            case ChunkMeshSelection::Opaque:
                if (mesh.opaque.idx_counter <= 0) {
                    return;
                }
                break;
            case ChunkMeshSelection::Transparent:
                if (mesh.transparent.idx_counter <= 0) {
                    return;
                }
                break;
        }

        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate(
                {static_cast<float>(cX * 16), static_cast<float>(cY * 16), static_cast<float>(cZ * 16)});
        Rendering::RenderContext::get().matrix_scale({32768.0f, 32768.0f /  32.0f, 32768.0f});

        switch (selection) {
            case ChunkMeshSelection::Opaque:
                mesh.opaque.mesh.draw();
                break;
            case ChunkMeshSelection::Transparent:
                mesh.transparent.mesh.draw();
                break;
        }
    }

    void ChunkMesh::add_block_to_mesh(const WorldData *wd, block_t block, const mathfu::Vector<int, 3> &vector3,
                                      const SurroundingPositions &positions) {

        try_add_face(wd, topFace, block, vector3, positions.up, LIGHT_TOP);
        try_add_face(wd, bottomFace, block, vector3, positions.down, LIGHT_BOT);

        try_add_face(wd, leftFace, block, vector3, positions.left, LIGHT_SIDE_X);
        try_add_face(wd, rightFace, block, vector3, positions.right, LIGHT_SIDE_X);

        try_add_face(wd, frontFace, block, vector3, positions.front, LIGHT_SIDE_Z);
        try_add_face(wd, backFace, block, vector3, positions.back, LIGHT_SIDE_Z);
    }

    void ChunkMesh::try_add_face(const WorldData *wd, const std::array<float, 12> &data, block_t block,
                                 const mathfu::Vector<int, 3> &actual_pos, const mathfu::Vector<int, 3> &check_pos,
                                 uint32_t lightValue) {
        int offsetX = cX * 16;
        int offsetY = cY * 16;
        int offsetZ = cZ * 16;

        int check_pos_wx = check_pos.x + offsetX;
        int check_pos_wy = check_pos.y + offsetY;
        int check_pos_wz = check_pos.z + offsetZ;

        if (check_pos_wx < 0 || check_pos_wx >= static_cast<int>(wd->x) ||
            check_pos_wy < 0 || check_pos_wy >= static_cast<int>(wd->y) ||
            check_pos_wz < 0 || check_pos_wz >= static_cast<int>(wd->z)) {
            return;
        }

        size_t index = CC_WIDX(check_pos_wx, check_pos_wy, check_pos_wz, wd);
        auto block_check = wd->blocks[index];

        if (block_check == BLK_Air || block_check == BLK_Water
        || block_check == BLK_Flower1 || block_check == BLK_Flower2
        || block_check == BLK_Mushroom1 || block_check == BLK_Mushroom2
        || block_check == BLK_Slab
#ifndef PSP
        || block_check == BLK_Leaves
#endif
        ) {
            if (block == BLK_Water && block_check != BLK_Water && lightValue == LIGHT_TOP) {
                add_face_to_mesh(waterTopFace, getTexCoord(block, lightValue), actual_pos, lightValue, mesh.transparent);
            } else if (block == BLK_Leaves) {
                add_face_to_mesh(data, getTexCoord(block, lightValue), actual_pos, lightValue, mesh.opaque);
            } else if (block != BLK_Water && block != BLK_Glass) {
                add_face_to_mesh(data, getTexCoord(block, lightValue), actual_pos, lightValue, mesh.opaque);
            }
        }
    }

    const std::array<uint16_t, 6> indexOrder = {0, 1, 2, 2, 3, 0};

    uint16_t convertRGBA8toRGBA4(uint32_t rgba8) {
        uint16_t r = (rgba8 >> 24) & 0xFF; // Extract 8-bit red
        uint16_t g = (rgba8 >> 16) & 0xFF; // Extract 8-bit green
        uint16_t b = (rgba8 >> 8) & 0xFF;  // Extract 8-bit blue
        uint16_t a = rgba8 & 0xFF;         // Extract 8-bit alpha

        // Convert them into 4-bit by taking the higher 4 bits of each 8-bit value
        r = r >> 4;
        g = g >> 4;
        b = b >> 4;
        a = a >> 4;

        // Pack them into a 16-bit value
        return (r << 12) | (g << 8) | (b << 4) | a;
    }

    void
    ChunkMesh::add_face_to_mesh(const std::array<float, 12> &face, std::array<float, 8> tex,
                                const mathfu::Vector<int, 3> &position,
                                uint32_t value, ChunkMeshInstance &m) {
        constexpr uint16_t maxTextureCoordinate = 32768;
        constexpr uint16_t scaleFactor = 32;
        auto newColor = convertRGBA8toRGBA4(value);

        for (size_t i = 0, tx = 0, idx = 0; i < 4; i++) {
            m.mesh.vertices.emplace_back(
                    static_cast<uint16_t>(tex[tx++] * maxTextureCoordinate),
                    static_cast<uint16_t>(tex[tx++] * maxTextureCoordinate),
                    newColor,
                    face[idx++] + position.x,
                    (face[idx++] + position.y) * scaleFactor,
                    face[idx++] + position.z
            );
        }

        for(size_t i = 0; i < 6; i++) {
            m.mesh.indices.emplace_back(m.idx_counter + indexOrder[i]);
        }
        m.idx_counter += 4;
    }

}