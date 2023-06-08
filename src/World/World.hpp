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
    private:

        void check_chunk_update();
        void update_chunks_list();

        std::map<uint64_t, ChunkStack*> chunks;
        Math::Vector2<int> player_chunk;
    };
}