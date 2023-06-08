#pragma once
#include <array>
#include <cstdint>
#include <Math/Math.hpp>
#include <CC/core.h>
#include <Rendering/GI.hpp>
#include <Rendering/Mesh.hpp>

namespace CrossCraft {
    using namespace Stardust_Celeste;
    using namespace Stardust_Celeste::Math;

    struct SurroundingPositions {
        Math::Vector3<int> up;
        Math::Vector3<int> down;
        Math::Vector3<int> left;
        Math::Vector3<int> right;
        Math::Vector3<int> front;
        Math::Vector3<int> back;

        inline void set(int x, int y, int z) {
            up = Math::Vector3<int>(x, y + 1, z);
            down = Math::Vector3<int>(x, y - 1, z);
            left = Math::Vector3<int>(x - 1, y, z);
            right = Math::Vector3<int>(x + 1, y, z);
            front = Math::Vector3<int>(x, y, z + 1);
            back = Math::Vector3<int>(x, y, z - 1);
        }
    };

    struct ChunkMeshInstance {

        inline auto delete_data() -> void{
            mesh.delete_data();
            idx_counter = 0;
            mesh.vertices.clear();
            mesh.indices.clear();
            mesh.vertices.shrink_to_fit();
            mesh.indices.shrink_to_fit();
        }

        inline auto pre_allocate() -> void {
            const int numFace = 8192;
            mesh.vertices.reserve(numFace * 4);
            mesh.indices.reserve(numFace * 6);
        }

        inline auto finalize() -> void {
            mesh.vertices.shrink_to_fit();
            mesh.indices.shrink_to_fit();
            mesh.setup_buffer();
        }

        uint16_t idx_counter;
        Rendering::Mesh<Rendering::Vertex> mesh;
    };

    enum ChunkMeshSelection {
        Opaque,
        Transparent
    };

    struct ChunkMeshCollection {
        ChunkMeshInstance opaque;
        ChunkMeshInstance transparent;
    };

    /**
 * @brief Get the Texture coords
 *
 * @param sideCount Number of tiles on XY
 * @param index Index of tile to get
 * @return std::array<float, 8>
 */
    inline auto getTexture(Math::Vector2<float> sideCount, int index) -> std::array<float, 8> {
        int row = index / (int)sideCount.x;
        int column = index % (int)sideCount.y;

        float sizeX = 1.f / ((float)sideCount.x);
        float sizeY = 1.f / ((float)sideCount.y);
        float y = (float)row * sizeY;
        float x = (float)column * sizeX;
        float h = y + sizeY;
        float w = x + sizeX;

        return {x, h, w, h, w, y, x, y};
    }

/*
0   Air             X
1   Stone           X
2   Dirt            X
3   Grass           X
4   Cobblestone     X
5   Wood            X
6   Sapling         X
7   Bedrock         X
8   Water           X
9   Still Water     X
10  Lava            X
11  Still Lava      X
12  Sand            X
13  Gravel          X
14  Gold Ore        X
15  Iron Ore        X
16  Coal Ore        X
17  Logs            X
18  Leaves          X
19  Sponge          X
20  Glass           X
21 - 36 Wool variants
37  Flower1         X
38  Flower2         X
39  Mushroom1       X
40  Mushroom2       X
41  Gold            X
42  Iron            X
43  Double Slab     X
44  Slab            X
45  Brick           X
46  TNT             X
47  Bookshelf       X
48  Mossy Rocks     X
49  Obsidian        X
*/

/**
 * @brief Get the Tex Coord from a block ID and lightvalue
 *
 * @param idx Index
 * @param lv Light Value
 * @return std::array<float, 8>
 */
    inline std::array<float, 8> getTexCoord(uint8_t idx, uint32_t lv) {
        auto vec = Math::Vector2<float>(16, 16);

        switch (idx) {
            case 1: // Stone
                return getTexture(vec, 1);
            case 3: // Dirt
                return getTexture(vec, 2);
            case 2: { // Grass
                if (lv == LIGHT_SIDE_X || lv == LIGHT_SIDE_Z)
                    return getTexture(vec, 3);
                else if (lv == LIGHT_BOT)
                    return getTexture(vec, 2);
                else
                    return getTexture(vec, 0);
            }
            case 4: // Cobblestone
                return getTexture(vec, 16);
            case 5: // Wood
                return getTexture(vec, 4);
            case 7: // Bedrock
                return getTexture(vec, 17);
            case 8:
            case 9: // Water
                return getTexture(vec, 14);
            case 10:
            case 11: // Lava
                return getTexture(vec, 30);
            case 12: // Sand
                return getTexture(vec, 18);
            case 13: // Gravel
                return getTexture(vec, 19);
            case 14: // Gold Ore
                return getTexture(vec, 32);
            case 15: // Iron Ore
                return getTexture(vec, 33);
            case 16: // Coal Ore
                return getTexture(vec, 34);
            case 6: // Sapling
                return getTexture(vec, 15);
            case 17: { // Log
                if (lv == LIGHT_TOP || lv == LIGHT_BOT)
                    return getTexture(vec, 21);
                else
                    return getTexture(vec, 20);
            }
            case 18: // Leaves
                return getTexture(vec, 51);
            case 19: // Sponge
                return getTexture(vec, 48);
            case 20: // Glass
                return getTexture(vec, 49);
            case 37: // Flower1
                return getTexture(vec, 13);
            case 38: // Flower2
                return getTexture(vec, 12);
            case 39: // Mushroom1
                return getTexture(vec, 29);
            case 40: // Mushroom2
                return getTexture(vec, 28);
            case 41: { // Gold
                if (lv == LIGHT_SIDE_X || lv == LIGHT_SIDE_Z)
                    return getTexture(vec, 40);
                else if (lv == LIGHT_BOT)
                    return getTexture(vec, 56);
                else
                    return getTexture(vec, 24);
            }
            case 42: { // Iron
                if (lv == LIGHT_SIDE_X || lv == LIGHT_SIDE_Z)
                    return getTexture(vec, 39);
                else if (lv == LIGHT_BOT)
                    return getTexture(vec, 55);
                else
                    return getTexture(vec, 23);
            }
            case 43: // DSLAB
                return getTexture(vec, 5);
            case 44: { // HSLAB
                if (lv == LIGHT_SIDE_X || lv == LIGHT_SIDE_Z) {
                    auto uv = getTexture(vec, 5);
                    //{0, 1, 2, 3, 4, 5, 6, 7}
                    //{x, h, w, h, w, y, x, y};
                    return {uv[0], uv[1] * 0.5f, uv[2], uv[3] * 0.5f,
                            uv[4], uv[5],        uv[6], uv[7]};
                } else
                    return getTexture(vec, 6);
            }
            case 45: // Brick
                return getTexture(vec, 7);
            case 46: // TNT
            {
                if (lv == LIGHT_SIDE_X || lv == LIGHT_SIDE_Z)
                    return getTexture(vec, 8);
                else if (lv == LIGHT_BOT)
                    return getTexture(vec, 10);
                else
                    return getTexture(vec, 9);
            }
            case 47: { // BookShelf
                if (lv == LIGHT_TOP || lv == LIGHT_BOT)
                    return getTexture(vec, 4);
                else
                    return getTexture(vec, 35);
            }
            case 48: // Mossy Cobble
                return getTexture(vec, 36);

            case 49: // Obsidian
                return getTexture(vec, 37);

            default:
                return getTexture(vec, idx + 43);
        }
    }
}
