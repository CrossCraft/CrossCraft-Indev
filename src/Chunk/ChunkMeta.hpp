#pragma once

namespace CrossCraft {
    struct ChunkMetaData {
        bool isEmpty = true;
        bool isFull = false;
    };

    class ChunkMeta {
    public:
        ChunkMeta() = default;
        ~ChunkMeta() = default;

        inline static auto get() -> ChunkMeta& {
            static ChunkMeta meta;
            return meta;
        }

        inline static auto getData(int x, int y, int z) -> ChunkMetaData& {
            return get().data[x][y][z];
        }

        void generate();
    private:
        ChunkMetaData data[32][4][32];
    };
}