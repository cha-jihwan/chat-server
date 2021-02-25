#pragma once

namespace c2 { namespace server { namespace contents
{
	// 전방 선언.
	class chat_user;

	// 채팅 룸 클래스.
	// 
	class chat_room
	{
	public:
		chat_room();
		~chat_room();

		string get_user_list_in_room_to_string();

		// user를 받아 들인다.
		void accept_user(chat_user* user);
		// user를 내보낸다.
		void export_user(chat_user* user);
		// 방에 있는 유저들에게 msg를 브로드 캐스팅 해준다.
		void broadcast(char* msg, size_t msg_size);

		// getter
		size_t get_size();
		size_t get_capacity();
		const string& get_name();

		//setter
		void set_name(const std::string& name);


	private:
		// user table 
		unordered_map<string, chat_user*> user_table;
		string							  name;
	};

} // namespace contents
} // namespace server
} // namespace c2