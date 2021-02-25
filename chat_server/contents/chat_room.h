#pragma once

namespace c2 { namespace server { namespace contents
{
	class chat_user;
	class chat_room
	{
	public:
		chat_room();
		~chat_room();

		string get_user_list_in_room_to_string();

		//getter
		const string& get_name();

		//setter
		void set_name(const std::string& name);

		void accept_user(chat_user* user);
		void export_user(chat_user* user);
		
		void broadcast(char* msg, size_t msg_size);

		size_t get_size();
		size_t get_capacity();

	private:
		unordered_map<string, chat_user*> user_table;
		string							  name;
	};

} // namespace contents
} // namespace server
} // namespace c2