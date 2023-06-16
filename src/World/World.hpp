#pragma once

#include <CC/core.h>
#include <Chunk/ChunkStack.hpp>
#include <map>

namespace CrossCraft {

    class World {
    public:
        World();

        ~World();

        void update(double dt);

        void draw();

        /// This function is called when a block is updated
        /// It updates the chunk that the block is in, and the 6 possible surrounding chunks
        /// This is done to ensure that the mesh is updated correctly
        /// @param x The x position of the block
        /// @param y The y position of the block
        /// @param z The z position of the block
        void handle_block_update(uint32_t x, uint32_t y, uint32_t z);

        static void handle_spawn_item(int16_t eid, float x, float y, float z, float vx, float vy, float vz, Item item);

        static auto save(const std::any p) -> void;

    private:

        void check_chunk_update();

        void update_chunks_list();

        std::map<uint64_t, ChunkStack *> chunks;
        mathfu::Vector<int, 2> player_chunk;
        uint32_t terrainTexID;
    };
}