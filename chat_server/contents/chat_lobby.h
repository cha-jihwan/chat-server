#pragma once

namespace c2 { namespace server { namespace contents
{
	// 전방 선언
	class chat_user;

	// 채팅 로비 공간 클래스.
	// 
	class chat_lobby
	{
	public:
		// 생성자
		chat_lobby();
		// 소멸자
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