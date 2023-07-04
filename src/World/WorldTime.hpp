#pragma once
#include <cstdint>

namespace CrossCraft
{

struct WorldTime {
	WorldTime() : tickTime(0) {};
	~WorldTime() = default;

	uint64_t tickTime;

	inline static auto get() -> WorldTime &
	{
		static WorldTime instance;
		return instance;
	}
};

}