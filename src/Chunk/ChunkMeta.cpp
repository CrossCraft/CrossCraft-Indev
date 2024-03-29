#include <Chunk/ChunkMeta.hpp>
#include <CC/world.h>

namespace CrossCraft
{
bool checkChunkEmpty(int cX, int cY, int cZ)
{
	auto wd = CC_World_GetData();

	for (int x = 0; x < 16; x++) {
		for (int z = 0; z < 16; z++) {
			for (int y = 0; y < 16; y++) {
				auto index = CC_WIDX(x + cX * 16, y + cY * 16,
						     z + cZ * 16, wd);
				if (wd->blocks[index] != 0) {
					return false;
				}
			}
		}
	}

	return true;
}

void ChunkMeta::generate()
{
    auto wd = CC_World_GetData();

    for (int x = 0; x < wd->x/16; x++) {
		for (int z = 0; z < wd->z/16; z++) {
			for (int y = 0; y < wd->y/16; y++) {
				data[x][y][z].isEmpty =
					checkChunkEmpty(x, y, z);
			}
		}
	}
}
}