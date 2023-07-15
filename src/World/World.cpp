#include <World/World.hpp>
#include <vector>
#include <Entity/ItemEntity.hpp>
#include <Entity/EntityManager.hpp>
#include <ResourcePack.hpp>
#include <Rendering/Texture.hpp>
#include <Chunk/Frustum.hpp>
#include <ModelRenderer.hpp>
#include <Chunk/ChunkMeta.hpp>
#include "WorldTime.hpp"

bool firstRun = true;

namespace CrossCraft
{

World::World()
{
	player_chunk = mathfu::Vector<int, 2>(-1, -1);

	terrainTexID = ResourcePack::get().get_texture("terrain");
}

World::~World() = default;

#if PSP
const int CHUNK_RADIUS_OPTION = 3;
#else
const int CHUNK_RADIUS_OPTION = 8;
#endif
constexpr int CHUNK_RADIUS_REAL =
	CHUNK_RADIUS_OPTION; // TODO: This should be +1 in infdev+

void World::update_chunks_list()
{
	int startX = player_chunk.x - CHUNK_RADIUS_REAL;
	int startZ = player_chunk.y - CHUNK_RADIUS_REAL;

	int endX = player_chunk.x + CHUNK_RADIUS_REAL + 1;
	int endZ = player_chunk.y + CHUNK_RADIUS_REAL + 1;

	std::unordered_map<uint64_t, ChunkStack *> new_chunks;

	if (startX < 0)
		startX = 0;
	if (startZ < 0)
		startZ = 0;

	size_t maxX, maxY, maxZ;
	CC_World_GetChunkSize(&maxX, &maxY, &maxZ);

	if ((size_t)endX > maxX)
		endX = maxX;
	if ((size_t)endZ > maxZ)
		endZ = maxZ;

	for (int x = startX; x < endX; x++) {
		for (int z = startZ; z < endZ; z++) {
			mathfu::Vector<int, 2> pos(x, z);

			// SQRT 2 = 1.41421356237
			// SQRT 2 / 2 = 0.70710678118
			if (CHUNK_RADIUS_REAL >= 3) {
				auto fpos = mathfu::Vector<float, 2>(x, z);
				auto fppos = mathfu::Vector<float, 2>(
					player_chunk.x, player_chunk.y);
				auto dist = (fpos - fppos).Length();
				if (dist > CHUNK_RADIUS_REAL + 0.5f)
					continue;
			}

			uint64_t id = ((uint64_t)pos.x) << 32 | (uint64_t)pos.y;

			if (chunks.find(id) == chunks.end()) {
				auto chk = new ChunkStack(x, z);
				new_chunks.emplace(id, chk);

				if(firstRun) {
					chk->prepare();
					chk->generate();
					chk->finalize();
				}
			} else {
				new_chunks.emplace(id, chunks[id]);
				chunks.erase(id);
			}
		}
	}

	for (auto &chunk : chunks) {
		delete chunk.second;
	}

	if(firstRun) {
		firstRun = false;
	}
	chunks = new_chunks;
}

void World::check_chunk_update()
{
	float x, y, z;
	// Get the player position
	CC_Player_GetPosition(&x, &y, &z);

	int cX = static_cast<int>(x) >> 4;
	int cZ = static_cast<int>(z) >> 4;

	// If the player has moved to a new chunk, update the chunks list
	if (cX != player_chunk.x || cZ != player_chunk.y) {
		player_chunk.x = cX;
		player_chunk.y = cZ;

		update_chunks_list();
	}

	// Light levels
	auto newLevel = WorldTime::get().internalLightLevel;
	if(newLevel != lastLightLevel) {
		lastLightLevel = newLevel;
		for(auto &[val, chunk] : chunks) {
			chunk->mark_dirty();
		}
	}
}

void World::update(double dt)
{
	check_chunk_update();
}

void World::draw()
{
	reset_chunk_frame();

	// Configure the GI
	GI::enable(GI_BLEND);
	GI::blend_func(GI_SRC_ALPHA, GI_ONE_MINUS_SRC_ALPHA);
	GI::enable(GI_ALPHA_TEST);
	GI::alpha_func(GI_GREATER, 0x20, 0xFF);

#if BUILD_PSP
	// PSP should render opaque
	GI::disable(GI_BLEND);
	GI::disable(GI_ALPHA_TEST);
#endif

	Frustum::update();

	Rendering::TextureManager::get().bind_texture(terrainTexID);
	for (auto &[val, chunk] : chunks) {
		chunk->draw(ChunkMeshSelection::Opaque);
	}

#if BUILD_PSP
	// PSP should render transparent
	GI::enable(GI_BLEND);
	GI::enable(GI_ALPHA_TEST);
#endif

	EntityManager::get().draw();
	ModelRenderer::get().draw_break();
	ModelRenderer::get().draw_block_outline();

	// This is done to ensure that the transparent blocks are drawn after the opaque blocks
	for (auto &[val, chunk] : chunks) {
		chunk->draw(ChunkMeshSelection::Transparent);
	}
}

// This function generates a chunk vector from a block position
auto generate_chunk_vector(uint32_t x, uint32_t y, uint32_t z)
	-> mathfu::Vector<int, 3>
{
	return { static_cast<int>(x >> 4), static_cast<int>(y >> 4),
		 static_cast<int>(z >> 4) };
}

void World::handle_block_update(uint32_t x, uint32_t y, uint32_t z)
{
	std::vector<mathfu::Vector<int, 3> > chunks_to_update_vec;
	chunks_to_update_vec.reserve(7);

	auto pref = generate_chunk_vector(x, y, z);
	preferredChunkPos = pref;

	// This updates the chunk that the block is in, and checks the 6 possible surrounding chunks
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y + 16, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y - 16, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y, z - 16));

	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y + 16, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y + 16, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y - 16, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y - 16, z));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y, z - 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y, z - 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y + 16, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y - 16, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y + 16, z - 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x, y - 16, z - 16));

	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y + 16, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y + 16, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y - 16, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y - 16, z + 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y + 16, z - 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y + 16, z - 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x + 16, y - 16, z - 16));
	chunks_to_update_vec.emplace_back(generate_chunk_vector(x - 16, y - 16, z - 16));


	// Update the chunks
	for (auto &chunk : chunks_to_update_vec) {
		uint64_t id = ((uint64_t)chunk.x) << 32 | (uint64_t)chunk.z;

		if (chunks.find(id) != chunks.end()) {
			auto cptr = chunks[id]->chunks[chunk.y];
			cptr->dirty = true;
			ChunkMeta::getData(cptr->cX, cptr->cY, cptr->cZ)
				.isEmpty = false;
		}
	}
}

void World::handle_spawn_item(int16_t eid, float x, float y, float z, float vx,
			      float vy, float vz, Item item)
{
	auto entity = new ItemEntity();
	entity->eid = eid;
	entity->data = new Item();
	entity->data->item_id = item.item_id;
	entity->data->damage = item.damage;
	entity->data->count = item.count;

	entity->position = mathfu::Vector<float, 3>(x, y, z);
	entity->velocity = mathfu::Vector<float, 3>(vx, vy, vz);
	entity->rotation = mathfu::Vector<float, 3>(0, 0, 0);

	EntityManager::get().add(entity);
}

auto World::save(const std::any p) -> void
{
	CC_World_Save();
}
}