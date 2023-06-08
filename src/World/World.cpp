#include <World/World.hpp>

namespace CrossCraft {

    World::World() {
        player_chunk = Math::Vector2<int>(-1, -1);
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

        if(endX > maxX) endX = maxX;
        if(endZ > maxZ) endZ = maxZ;

        SC_APP_DEBUG("SCANNING CHUNKS {0}, {1} -> {2}, {3}", startX, startZ, endX, endZ);

        for(int x = startX; x < endX; x++) {
            for(int z = startZ; z < endZ; z++) {
                Math::Vector2<int> pos(x, z);

                uint64_t id = ((uint64_t)pos.x) << 32 | (uint64_t)pos.y;

                if(chunks.find(id) == chunks.end()) {
                    SC_APP_DEBUG("NEW CHUNK {0}, {1}", x, z);
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
        CC_Player_GetPosition(&x, &y, &z);


        int cX = static_cast<int>(x) >> 4;
        int cZ = static_cast<int>(z) >> 4;

        if(cX != player_chunk.x || cZ != player_chunk.y) {

            SC_APP_DEBUG("PPOS {0}, {1}, {2}", x, y, z);
            SC_APP_DEBUG("PCHUNK {0}, {1}", cX, cZ);

            player_chunk.x = cX;
            player_chunk.y = cZ;

            //Oopsie!
            update_chunks_list();
        }
    }

    void World::update(double dt) {
        check_chunk_update();
    }

    void World::draw() {
        for(auto& [val, chunk] : chunks) {
            chunk->draw(ChunkMeshSelection::Opaque);
        }

        for(auto& [val, chunk] : chunks) {
            chunk->draw(ChunkMeshSelection::Transparent);
        }
    }
}