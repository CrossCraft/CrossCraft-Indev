#pragma once
#include <cstdint>

namespace CrossCraft
{

struct WorldTime {
	WorldTime() : tickTime(0) {};
	~WorldTime() = default;

	uint64_t tickTime;
	uint8_t internalLightLevel = 15;

	inline static auto get() -> WorldTime &
	{
		static WorldTime instance;
		return instance;
	}
};

}