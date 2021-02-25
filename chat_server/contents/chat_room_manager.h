#pragma once

namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core::constant;

	// 방 관리자 클래스.
	// 모든(할당 + 비할) 방을 관리한다. 
	class chat_room_manager
	{
	public:
		// 생성자
		chat_room_manager();
		// 소멸자
		~chat_room_manager();

		// 방 할당자.
		chat_room* allocate(const string& room_name);
		void free(chat_room* room);


		// 방 이름을 통해 방을 찾습니다.
		// param name : 방 이름
		// return value 
		// if	nullptr 이름에 맞는 방이 없는 경우
		// else			찾은 방을 리턴
		chat_room* find_room_using_name(const string& name);
	
		// 방 목록을 문자열로 "room_name" + "\r\n" 반환해준다.
		string get_active_room_list_to_string();

	private:
		// 할당된 방을 관리하는 자료구조
		unordered_map<string, chat_room*> active_room_table;
		// 오브젝트 풀
		bounded_object_pool<chat_room, c_maximum_ccu> room_pool;
	};

	extern chat_room_manager* g_room_manager;
} // namespace contents
} // namespace server
} // namespace c2