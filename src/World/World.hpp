#pragma once
#include <CC/core.h>
#include <Chunk/ChunkStack.hpp>
#include <map>

namespace CrossCraft{

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

        static auto save(std::any p) -> void;
    private:

        void check_chunk_update();
        void update_chunks_list();

        std::map<uint64_t, ChunkStack*> chunks;
        Math::Vector2<int> player_chunk;
        uint32_t terrainTexID;
    };
}