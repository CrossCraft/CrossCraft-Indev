#include <World/World.hpp>
#include <vector>
#include <algorithm>
#include <ResourcePack.hpp>
#include <Rendering/Texture.hpp>

namespace CrossCraft {

    World::World() {
        player_chunk = mathfu::Vector<int, 2>(-1, -1);

        terrainTexID = ResourcePack::get().get_texture("terrain");
        SC_APP_INFO("Terrain texture ID: {0}", terrainTexID);
    }

    World::~World() {

    }

    const int CHUNK_RADIUS_OPTION = 4;
    constexpr int CHUNK_RADIUS_REAL = CHUNK_RADIUS_OPTION + 1;

    void World::update_chunks_list() {
        int startX = player_chunk.x - CHUNK_RADIUS_REAL;
        int startZ = player_chunk.y - CHUNK_RADIUS_REAL;

        int endX = player_chunk.x + CHUNK_RADIUS_REAL + 1;
        int endZ = player_chunk.y + CHUNK_RADIUS_REAL + 1;

        std::map<uint64_t, ChunkStack*> new_chunks;

        if(startX < 0) startX = 0;
        if(startZ < 0) startZ = 0;

        size_t maxX, maxY, maxZ;
        CC_World_GetChunkSize(&maxX, &maxY, &maxZ);

        if((size_t)endX > maxX) endX = maxX;
        if((size_t)endZ > maxZ) endZ = maxZ;

        //SC_APP_DEBUG("SCANNING CHUNKS {0}, {1} -> {2}, {3}", startX, startZ, endX, endZ);

        for(int x = startX; x < endX; x++) {
            for(int z = startZ; z < endZ; z++) {
                mathfu::Vector<int, 2> pos(x, z);

                uint64_t id = ((uint64_t)pos.x) << 32 | (uint64_t)pos.y;

                if(chunks.find(id) == chunks.end()) {
                    //SC_APP_DEBUG("NEW CHUNK {0}, {1}", x, z);
                    new_chunks.emplace(id, new ChunkStack(x, z));
                } else {
                    new_chunks.emplace(id, chunks[id]);
                    chunks.erase(id);
                }
            }
        }

        for(auto& chunk : chunks) {
            delete chunk.second;
        }

        chunks = new_chunks;
    }

    void World::check_chunk_update() {
        float x, y, z;
        // Get the player position
        CC_Player_GetPosition(&x, &y, &z);


        int cX = static_cast<int>(x) >> 4;
        int cZ = static_cast<int>(z) >> 4;

        // If the player has moved to a new chunk, update the chunks list
        if(cX != player_chunk.x || cZ != player_chunk.y) {

            player_chunk.x = cX;
            player_chunk.y = cZ;

            update_chunks_list();
        }
    }

    void World::update(double dt) {
        check_chunk_update();
    }

    void World::draw() {
        Rendering::TextureManager::get().bind_texture(terrainTexID);
        for(auto& [val, chunk] : chunks) {
            chunk->draw(ChunkMeshSelection::Opaque);
        }

        // This is done to ensure that the transparent blocks are drawn after the opaque blocks
        for(auto& [val, chunk] : chunks) {
            chunk->draw(ChunkMeshSelection::Transparent);
        }
    }

    // This function generates a chunk vector from a block position
    auto generate_chunk_vector(uint32_t x, uint32_t y, uint32_t z) -> mathfu::Vector<int, 3> {
        return mathfu::Vector<int, 3>(x >> 4, y >> 4, z >> 4);
    }


    void World::handle_block_update(uint32_t x, uint32_t y, uint32_t z) {
        std::vector<mathfu::Vector<int, 3>> chunks_to_update_vec;
        chunks_to_update_vec.reserve(7);

        // This updates the chunk that the block is in, and checks the 6 possible surrounding chunks
        chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y, z));
        chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 1, y, z));
        chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 1, y, z));
        chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y + 1, z));
        chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y - 1, z));
        chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y, z + 1));
        chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y, z - 1));

        // Update the chunks
        for(auto& chunk : chunks_to_update_vec) {
            uint64_t id = ((uint64_t)chunk.x) << 32 | (uint64_t)chunk.z;

            if(chunks.find(id) != chunks.end()) {
                chunks[id]->chunks[chunk.y]->dirty = true;
            }
        }

    }


    auto World::save(std::any p) -> void {
        CC_World_Save();
    }
}