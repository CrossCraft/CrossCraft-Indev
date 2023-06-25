#include <Chunk/ChunkMesh.hpp>
#include <Chunk/FaceConst.hpp>
#include <Chunk/Frustum.hpp>
#include <Chunk/ChunkMeta.hpp>

namespace CrossCraft
{

static int chunksLastFrame = 0;
void reset_chunk_frame() {
	chunksLastFrame = 0;
}

ChunkMesh::ChunkMesh(int x, int y, int z)
	: cX(x)
	, cY(y)
	, cZ(z)
{
	dirty = true;
}

ChunkMesh::~ChunkMesh() = default;

void ChunkMesh::finalize_mesh()
{
	// Finalize
	mesh.opaque.finalize();
	mesh.transparent.finalize();

	dirty = false;
}

void ChunkMesh::prepare_mesh()
{
	// Delete Data
	mesh.transparent.delete_data();
	mesh.opaque.delete_data();
	mesh.opaque.pre_allocate();
}

void ChunkMesh::generate_mesh()
{
	SurroundingPositions surround_pos;
	uint32_t index;
	block_t block;
	mathfu::Vector<int, 3> v;
	auto wd = CC_World_GetData();

	// Check if chunk is empty
	if (ChunkMeta::getData(cX, cY, cZ).isEmpty) {
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

				if (block == BLK_Flower1 ||
				    block == BLK_Flower2 ||
				    block == BLK_Mushroom1 ||
				    block == BLK_Mushroom2) {
					auto texCoord =
						getTexCoord(block, LIGHT_TOP);
					std::array<float, 8> texCoord2 = {
						texCoord[2], texCoord[3],
						texCoord[4], texCoord[5],
						texCoord[6], texCoord[7],
						texCoord[0], texCoord[1]
					};
					add_face_to_mesh(xFace1, texCoord, v,
							 LIGHT_TOP,
							 LIGHT_TOP, mesh.transparent);
					add_face_to_mesh(xFace2, texCoord, v,
							 LIGHT_TOP,
							 LIGHT_TOP, mesh.transparent);
					add_face_to_mesh(xFace3, texCoord2, v,
							 LIGHT_TOP,
							 LIGHT_TOP, mesh.transparent);
					add_face_to_mesh(xFace4, texCoord2, v,
							 LIGHT_TOP,
							 LIGHT_TOP, mesh.transparent);
					continue;
				}

				if (block == BLK_Slab) {
					add_face_to_mesh(
						topFaceHalf,
						getTexCoord(block, LIGHT_TOP),
						v, LIGHT_TOP, LIGHT_TOP, mesh.opaque);
					try_add_face(wd, bottomFace, block, v,
						     surround_pos.down,
						     LIGHT_BOT);

					try_add_face(wd, leftFaceHalf, block, v,
						     surround_pos.left,
						     LIGHT_SIDE_X);
					try_add_face(wd, rightFaceHalf, block,
						     v, surround_pos.right,
						     LIGHT_SIDE_X);

					try_add_face(wd, frontFaceHalf, block,
						     v, surround_pos.front,
						     LIGHT_SIDE_Z);
					try_add_face(wd, backFaceHalf, block, v,
						     surround_pos.back,
						     LIGHT_SIDE_Z);
					continue;
				}

				add_block_to_mesh(wd, block, v, surround_pos);
			}
		}
	}

end:
	return;
}

#if PSP
#define CHUNKS_PER_FRAME 1
#else
#define CHUNKS_PER_FRAME 16
#endif

void ChunkMesh::draw(CrossCraft::ChunkMeshSelection selection)
{
	if (!Frustum::get().IsBoxVisible(
		    mathfu::Vector<float, 3>(cX * 16, cY * 16, cZ * 16),
		    mathfu::Vector<float, 3>(cX * 16 + 16, cY * 16 + 16,
					     cZ * 16 + 16))) {
		return;
	}

	if (dirty && chunksLastFrame < CHUNKS_PER_FRAME) {
		chunksLastFrame++;
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
		{ static_cast<float>(cX * 16), static_cast<float>(cY * 16),
		  static_cast<float>(cZ * 16) });
	Rendering::RenderContext::get().matrix_scale(
		{ 32768.0f, 32768.0f / 32.0f, 32768.0f });

	switch (selection) {
	case ChunkMeshSelection::Opaque:
		mesh.opaque.mesh.draw();
		break;
	case ChunkMeshSelection::Transparent:
		mesh.transparent.mesh.draw();
		break;
	}
}

void ChunkMesh::add_block_to_mesh(const WorldData *wd, block_t block,
				  const mathfu::Vector<int, 3> &vector3,
				  const SurroundingPositions &positions)
{
	try_add_face(wd, topFace, block, vector3, positions.up, LIGHT_TOP);
	try_add_face(wd, bottomFace, block, vector3, positions.down, LIGHT_BOT);

	try_add_face(wd, leftFace, block, vector3, positions.left,
		     LIGHT_SIDE_X);
	try_add_face(wd, rightFace, block, vector3, positions.right,
		     LIGHT_SIDE_X);

	try_add_face(wd, frontFace, block, vector3, positions.front,
		     LIGHT_SIDE_Z);
	try_add_face(wd, backFace, block, vector3, positions.back,
		     LIGHT_SIDE_Z);
}

void ChunkMesh::try_add_face(const WorldData *wd,
			     const std::array<float, 12> &data, block_t block,
			     const mathfu::Vector<int, 3> &actual_pos,
			     const mathfu::Vector<int, 3> &check_pos,
			     uint32_t lightValue)
{
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

	if (block_check == BLK_Air || block_check == BLK_Water ||
	    block_check == BLK_Flower1 || block_check == BLK_Flower2 ||
	    block_check == BLK_Mushroom1 || block_check == BLK_Mushroom2 ||
	    block_check == BLK_Slab
#ifndef PSP
	    || block_check == BLK_Leaves
#endif
	) {
		u32 lightColorI = calculateLighting(wd, lightValue, index);

		if (block == BLK_Water && block_check != BLK_Water &&
		    lightValue == LIGHT_TOP) {
			add_face_to_mesh(waterTopFace,
					 getTexCoord(block, lightValue),
					 actual_pos, lightValue, lightColorI,
					 mesh.transparent);
		} else if (block == BLK_Leaves) {
			add_face_to_mesh(data, getTexCoord(block, lightValue),
					 actual_pos, lightValue, lightColorI, mesh.opaque);
		} else if (block != BLK_Water && block != BLK_Glass) {
			add_face_to_mesh(data, getTexCoord(block, lightValue),
					 actual_pos, lightValue, lightColorI, mesh.opaque);
		}
	}
}

int getAOY(int v, mathfu::Vector<int, 3> pos, ChunkMesh *chunkMesh, int off, int off2) {
	pos.x += chunkMesh->cX * 16;
	pos.y += chunkMesh->cY * 16;
	pos.z += chunkMesh->cZ * 16;

	pos.y += off;
	int x, z;
	// 01
	if (off2) {
		v = (v + off2 - 1) % 4;

		if (v == 0) {
			x = -1;
			z = 1;
		} else if (v == 1) {
			x = -1;
			z = -1;
		} else if (v == 2) {
			x = 1;
			z = -1;
		} else if (v == 3) {
			x = 1;
			z = 1;
		}
	} else {
		if (v == 0) {
			x = -1;
			z = 1;
		} else if (v == 1) {
			x = 1;
			z = 1;
		} else if (v == 2) {
			x = 1;
			z = -1;
		} else if (v == 3) {
			x = -1;
			z = -1;
		}
	}

	auto m = CC_World_GetData();

	block_t result;

	bool side1 = false;
	if (CC_World_TryGetBlock(pos.x + x, pos.y, pos.z, &result)) {
		if (result != 0) {
			side1 = true;
		}
	}

	bool side2 = false;
	if (CC_World_TryGetBlock(pos.x, pos.y, pos.z + z, &result)) {
		if (result != 0) {
			side2 = true;
		}
	}

	bool corner = false;
	if (CC_World_TryGetBlock(pos.x + x, pos.y, pos.z + z, &result)) {
		if (result != 0) {
			corner = true;
		}
	}

	int sum = 0;

	if (corner)
		sum += 1;

	if (side1)
		sum += 1;

	if (side2)
		sum += 1;

	return sum;
}

int getAOX(int v, mathfu::Vector<int, 3> pos, ChunkMesh *chunkMesh, int off, int off2) {
	v = (v + 1) % 4;
	pos.x += chunkMesh->cX * 16;
	pos.y += chunkMesh->cY * 16;
	pos.z += chunkMesh->cZ * 16;

	pos.z += off;
	int x, y;
	// 01
	if (off2) {
		v = (v + off2) % 4;

		if (v == 0) {
			x = -1;
			y = -1;
		} else if (v == 1) {
			x = -1;
			y = 1;
		} else if (v == 2) {
			x = 1;
			y = 1;
		} else if (v == 3) {
			x = 1;
			y = -1;
		}
	} else {
		if (v == 0) {
			x = -1;
			y = 1;
		} else if (v == 1) {
			x = -1;
			y = -1;
		} else if (v == 2) {
			x = 1;
			y = -1;
		} else if (v == 3) {
			x = 1;
			y = 1;
		}
	}

	auto wd = CC_World_GetData();

	bool side1 = false;
	block_t result;


	if (CC_World_TryGetBlock(pos.x + x, pos.y, pos.z, &result)) {
		if (result != 0) {
			side1 = true;
		}
	}

	bool side2 = false;
	if (CC_World_TryGetBlock(pos.x, pos.y + y, pos.z, &result)) {
		if (result != 0) {
			side2 = true;
		}
	}

	bool corner = false;
	if (CC_World_TryGetBlock(pos.x + x, pos.y + y, pos.z, &result)) {
		if (result != 0) {
			corner = true;
		}
	}

	int sum = 0;

	if (corner)
		sum += 1;

	if (side1)
		sum += 1;

	if (side2)
		sum += 1;

	return sum;
}

int getAOZ(int v, mathfu::Vector<int, 3> pos, ChunkMesh *chunkMesh, int off, int off2) {
	v = (v + 1) % 4;
	pos.x += chunkMesh->cX * 16;
	pos.y += chunkMesh->cY * 16;
	pos.z += chunkMesh->cZ * 16;

	pos.x += off;
	int z, y;
	// 01
	if (off2) {
		v = (v + off2) % 4;

		if (v == 0) {
			z = -1;
			y = -1;
		} else if (v == 1) {
			z = -1;
			y = 1;
		} else if (v == 2) {
			z = 1;
			y = 1;
		} else if (v == 3) {
			z = 1;
			y = -1;
		}
	} else {
		if (v == 0) {
			z = -1;
			y = 1;
		} else if (v == 1) {
			z = -1;
			y = -1;
		} else if (v == 2) {
			z = 1;
			y = -1;
		} else if (v == 3) {
			z = 1;
			y = 1;
		}
	}

	bool side1 = false;
	block_t result;

	if (CC_World_TryGetBlock(pos.x, pos.y, pos.z + z, &result)) {
		if (result != 0) {
			side1 = true;
		}
	}

	bool side2 = false;
	if (CC_World_TryGetBlock(pos.x, pos.y + y, pos.z, &result)) {
		if (result != 0) {
			side2 = true;
		}
	}

	bool corner = false;
	if (CC_World_TryGetBlock(pos.x, pos.y + y, pos.z + z, &result)) {
		if (result != 0) {
			corner = true;
		}
	}

	int sum = 0;

	if (corner)
		sum += 1;

	if (side1)
		sum += 1;

	if (side2)
		sum += 1;

	return sum;
}

u32 ChunkMesh::calculateLighting(const WorldData *wd, uint32_t lightValue,
				 size_t index) const
{
	// Let's calculate lighting;
	mathfu::Vector<float, 3> light = { 0.25f, 0.25f, 0.25f };
	auto lv = wd->lightmap[index];

	uint8_t sky_light = lv >> 4;
	uint8_t block_light = lv & 0xF;
	float result = std::max(block_light, sky_light);

	light += mathfu::Vector<float, 3>(result / 15.0f, result / 15.0f, result / 15.0f) * 0.75f;
	if(lightValue == LIGHT_SIDE_X) {
		light *= 0.8f;
	} else if(lightValue == LIGHT_SIDE_Z) {
		light *= 0.6f;
	} else if(lightValue == LIGHT_BOT) {
		light *= 0.5f;
	}

	auto lc = mathfu::Vector<float, 4>(light.x, light.y, light.z, 1.0f);
	auto lightColor = Rendering::Color{{
			static_cast<uint8_t>(lc.x * 255.0f),
			static_cast<uint8_t>(lc.y * 255.0f),
			static_cast<uint8_t>(lc.z * 255.0f),
			static_cast<uint8_t>(lc.w * 255.0f)
		}};
	auto lightColorI = lightColor.color;
		return lightColorI;
}

const std::array<uint16_t, 6> indexOrder = { 0, 1, 2, 2, 3, 0 };

#define FANCY_LIGHTING 1

void ChunkMesh::add_face_to_mesh(const std::array<float, 12> &face,
				 std::array<float, 8> tex,
				 const mathfu::Vector<int, 3> &position,
				 uint32_t value, uint32_t value2, ChunkMeshInstance &m)
{
	constexpr uint16_t maxTextureCoordinate = 32768;
	constexpr uint16_t scaleFactor = 32;

	Rendering::Color c{};

	for (size_t i = 0, tx = 0, idx = 0; i < 4; i++) {
		c.color = value2;

#if FANCY_LIGHTING
		mathfu::Vector<float, 3> col = { (float)c.rgba.r, (float)c.rgba.g, (float)c.rgba.b };

		if (value == LIGHT_TOP)
			col *= (1.0f - (float)(getAOY(i, position, this, 1, 0)) * 0.15f);
		else if (value == LIGHT_BOT)
			col *= (1.0f - (float)(getAOY(i, position, this, -1, 2)) * 0.15f);
		else if (value == LIGHT_SIDE_Z)
			if (face[0] == 0 && face[2] == 1) {
				col *= (1.0f -(float)(getAOX(i, position, this, 1, 0)) * 0.15f);
			} else
				col *= (1.0f -(float)(getAOX(i, position, this, -1, 2)) * 0.15f);
		else if (value == LIGHT_SIDE_X)
			if (face[0] == 1 && face[2] == 1) {
				col *= (1.0f -(float)(getAOZ(i, position, this, 1, 2)) * 0.15f);
			} else
				col *= (1.0f -(float)(getAOZ(i, position, this, -1, 0)) * 0.15f);

		c = Rendering::Color{{static_cast<u8>(col.x), static_cast<u8>(col.y), static_cast<u8>(col.z), c.rgba.a}};
#endif

		m.mesh.vertices.emplace_back(
			static_cast<uint16_t>(tex[tx++] * maxTextureCoordinate),
			static_cast<uint16_t>(tex[tx++] * maxTextureCoordinate),
			c.color, face[idx++] + position.x,
			(face[idx++] + position.y) * scaleFactor,
			face[idx++] + position.z);
	}

	for (size_t i = 0; i < 6; i++) {
		m.mesh.indices.emplace_back(m.idx_counter + indexOrder[i]);
	}
	m.idx_counter += 4;
}

}