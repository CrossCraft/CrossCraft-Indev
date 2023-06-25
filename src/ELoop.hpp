#pragma once
#include <CC/eventloop.h>

namespace CrossCraft
{
struct ELoop {
	inline static auto get() -> ELoop &
	{
		static ELoop instance;
		return instance;
	}
	EventLoop *client_event_loop;
};
}