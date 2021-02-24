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

	chat_room* chat_room_manager::allocate()
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


	chat_room* chat_room_manager::find(const string& name)
	{
		if (0 < active_room_table.count(name))
		{
			return nullptr;
		}

		return active_room_table[name];
	}

	string&& chat_room_manager::get_active_room_list_to_string()
	{
		std::string room_list_str;

		for (const auto& kv : active_room_table)
		{
			crash_if_false(nullptr != kv.second);

			room_list_str += kv.second->get_name();
			room_list_str += "\r\n";
		}

		return std::move(room_list_str);
	}
} // namespace contents
} // namespace server
} // namespace c2