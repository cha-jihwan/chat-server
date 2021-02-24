#include "../common/pre_compile.h"
#pragma once

namespace c2 { namespace server { namespace contents
{
	chat_room_manager* g_room_manager {};

	chat_room_manager::chat_room_manager()
	{
	}

	chat_room_manager::~chat_room_manager()
	{
	}

	chat_room * chat_room_manager::allocate()
	{
		chat_room* chatting_room = room_pool.allocate();

		crash_if_false(nullptr != chatting_room);

		return chatting_room;
	}

	void chat_room_manager::free(chat_room* room)
	{
		crash_if_false(nullptr != room);

		room_pool.free(room);
	}
} // namespace contents
} // namespace server
} // namespace c2