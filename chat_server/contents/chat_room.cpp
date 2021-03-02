#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace contents
{
	chat_room::chat_room()
	{
	}

	chat_room::~chat_room()
	{
	}

	const string & chat_room::get_name()
	{
		return name;
	}

	string chat_room::get_user_list_in_room_to_string()
	{
		string user_list = packet_filter_keywords[(size_t)e_packet_filter::EPF_SELECT_USER_LIST_IN_ROOM];

		for (const auto& kv : user_table)
		{
			user_list += kv.first;
			user_list += "\r\n";
		}

		return std::move(user_list);
	}

	void chat_room::set_name(const std::string& name)
	{
		this->name = name;
	}

	void chat_room::accept_user(chat_user* user)
	{
		crash_if_false(nullptr != user);
		this->user_table.emplace(user->get_name(), user);
	}

	void chat_room::export_user(chat_user* user)
	{
		crash_if_false(nullptr != user);
		this->user_table.erase(user->get_name());
	}

	void chat_room::broadcast(char * msg, size_t msg_size)
	{
		for (const auto& kv : user_table)
		{
			crash_if_false(nullptr != kv.second);
			crash_if_false(nullptr != kv.second->get_session());

			if (msg_size != kv.second->get_session()->pre_send(msg, msg_size))
			{
				// error handling
			}
		}
	}

	size_t chat_room::get_size()
	{
		return user_table.size();
	}

	size_t chat_room::get_capacity()
	{
		return size_t();
	}

}// c2
}// server 
}// contents
