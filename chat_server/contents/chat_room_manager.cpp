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

	chat_room* chat_room_manager::allocate(const string& room_name)
	{
		chat_room* chatting_room = room_pool.allocate();

		crash_if_false(nullptr != chatting_room);

		active_room_table.emplace(room_name, chatting_room);

		return chatting_room;
	}

	void chat_room_manager::free(chat_room* room)
	{
		crash_if_false(nullptr != room);

		active_room_table.erase(room->get_name());

		room_pool.free(room);
	}



	chat_room* chat_room_manager::find_room_using_name(const string& name)
	{
		if (0 >= active_room_table.count(name))
		{
			return nullptr;
		}

		return active_room_table[name];
	}

	string chat_room_manager::get_active_room_list_to_string()
	{
		std::string room_list_str = packet_filter_keywords[(size_t)e_packet_filter::EPF_SELECT_ROOM_LIST];
		room_list_str + "\r\n";

		int room_no{ 1 };
		for (const auto& kv : active_room_table)
		{
			crash_if_false(nullptr != kv.second);

			room_list_str += std::to_string(room_no);
			room_list_str += "¹ø ¹æ : ";
			room_list_str += kv.second->get_name();
			room_list_str += "\r\n";

			room_no += 1;
		}

		return std::move(room_list_str);
	}

} // namespace contents
} // namespace server
} // namespace c2