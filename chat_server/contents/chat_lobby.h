#pragma once

namespace c2 { namespace server { namespace contents
{
	class chat_user;
	class chat_lobby
	{
	public:
		chat_lobby();
		~chat_lobby();

		void accept_user(chat_user* user);
		void export_user(chat_user* user);

		void broadcast(char* msg, size_t msg_size);
	private:
		std::unordered_map<std::string, chat_user*> named_users;
	};

	extern chat_lobby* g_lobby;
} // namespace contents
} // namespace server
} // namespace c2