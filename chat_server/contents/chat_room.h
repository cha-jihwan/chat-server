#pragma once

namespace c2 { namespace server { namespace contents
{
	class chat_user;
	class chat_room
	{
	public:
		chat_room();
		~chat_room();

		//getter
		const string& get_name();
		string&& get_user_list_in_room_to_string();
		//setter
		void set_name(std::string&& name);

		
		void broadcast(char* msg, size_t msg_size);
	private:
		unordered_map<string, chat_user*> user_table;
		string name;
	};

} // namespace contents
} // namespace server
} // namespace c2