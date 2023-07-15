#pragma once

#include <CC/core.h>
#include <Chunk/ChunkStack.hpp>
#include <unordered_map>

namespace CrossCraft
{

class World {
    public:
	World();

	~World();

	void update(double dt);

	void draw();

	inline static auto get() -> World &
	{
		static World instance;
		return instance;
	}

	void handle_block_update(uint32_t x, uint32_t y, uint32_t z);

	static void handle_spawn_item(int16_t eid, float x, float y, float z,
				      float vx, float vy, float vz, Item item);

	static auto save(const std::any p) -> void;

    private:
	void check_chunk_update();

	void update_chunks_list();

	std::unordered_map<uint64_t, ChunkStack *> chunks;
	mathfu::Vector<int, 2> player_chunk;
	uint32_t terrainTexID;
	uint8_t lastLightLevel = 0;
};
}