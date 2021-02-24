#pragma once

namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core::constant;

	class chat_room_manager
	{
	public:
		chat_room_manager();
		~chat_room_manager();

		chat_room* allocate();
		void free(chat_room* room);

		// return value 
		// if	nullptr 이름에 맞는 방이 없는 경우
		// else			찾은 방을 리턴
		//		
		chat_room* find(const string& name);
		string&& get_active_room_list_to_string();

	private:
		unordered_map<string, chat_room*> active_room_table;
		bounded_object_pool<chat_room, c_maximum_ccu> room_pool;
	};

	extern chat_room_manager* g_room_manager;
} // namespace contents
} // namespace server
} // namespace c2