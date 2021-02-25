#include "../common/pre_compile.h"

unordered_map<std::string, telnet_parser::packet_handler> telnet_parser::packet_handlers;

// 최소 명령어 크기 case 1: 1 + N + 2
// case 1:'\'(1) + cmd(가변 N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
// case 2:'\'(1) + cmd(가변 N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
// case 3:'\'(1) + cmd(가변 N) + 1(sapce) + 상대ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
// case 1: command\r\n
// case 2: command space body \r\n
// case 3: command space target_name body \r\n
size_t telnet_parser::parse_payload(c2::server::contents::chat_session* in_session, char* in_buffer, size_t in_size, std::vector<packet>& out_buffer)
{
	size_t	parsed_size{};

	for (;;)
	{
		char* buf_ptr = &in_buffer[parsed_size];
		// 남은 사이즈가 최소 명령 사이즈 보다 작다면 종료.
		// 4 = commnand_size (2) + \r\n (2)
		if (4 > in_size - parsed_size)
		{
			break;
		}

		char* buf_end = telnet_parser::find_enter_from_payload(buf_ptr, in_size - parsed_size);
		if (nullptr == buf_end) // 명령어 완성 X
		{
			break;
		}

		size_t current_cmd_size{(size_t)(buf_end + 2) - (size_t)buf_ptr};
		size_t return_size{};
		string cmd{buf_ptr, 2};

		// 1개도 없는 경우...
		if (1 <= packet_handlers.count(cmd))
		{
			return_size = packet_handlers[cmd](in_session, &in_buffer[parsed_size + 2], in_size - 2);
		}

		parsed_size += current_cmd_size;
	}

	return parsed_size;
}


void telnet_parser::initialize()
{
	//// 로그인
	packet_handlers.emplace("/L", telnet_parser::login_handler);
	packet_handlers.emplace("/l", telnet_parser::login_handler);

	//// 방 생성
	packet_handlers.emplace("/R", telnet_parser::create_room_handler);
	packet_handlers.emplace("/r", telnet_parser::create_room_handler);

	//// 방 접속
	packet_handlers.emplace("/Q", telnet_parser::enter_room_handler);
	packet_handlers.emplace("/q", telnet_parser::enter_room_handler);

	//// 방 나가기
	packet_handlers.emplace("/E", telnet_parser::leave_room_handler);
	packet_handlers.emplace("/e", telnet_parser::leave_room_handler);

	//// 채팅
	packet_handlers.emplace("/C", telnet_parser::chatting_handler);
	packet_handlers.emplace("/c", telnet_parser::chatting_handler);
	packet_handlers.emplace("/ㅊ", telnet_parser::chatting_handler);

	//// 룸 목록
	packet_handlers.emplace("/S", telnet_parser::select_rooms_handler);
	packet_handlers.emplace("/s", telnet_parser::select_rooms_handler);

	//// 귓속말
	packet_handlers.emplace("/W", telnet_parser::whisper_handler);
	packet_handlers.emplace("/w", telnet_parser::whisper_handler);

	//// 방 참여자 목록
	packet_handlers.emplace("/U", telnet_parser::select_user_in_room_handler);
	packet_handlers.emplace("/u", telnet_parser::select_user_in_room_handler);

	//// 접속자 목록
	packet_handlers.emplace("/A", telnet_parser::select_all_handler);
	packet_handlers.emplace("/a", telnet_parser::select_all_handler);
}

//
//// payload로 부터 문자를 찾는다.
//// return value 
//// 문자열을 찾는다면 시작지점
//// 못찾는다면 nullptr
//// 문자를 찾지 못한다면 nullptr
//// 문자를 찾는다면 위치 시작 지점.
char* telnet_parser::find_space_from_payload(char* in_buf, size_t in_size)
{
	for (size_t n{}; n < in_size; ++n)
	{
		if (in_buf[n] == ' ')
		{
			return &in_buf[n];
		}
	}

	return nullptr;
}





//// payload로 부터 telnet의 enter입력 (linfeed + carage-return)를 찾는다.
//// \r\n 
//// return value 
//// 문자열을 찾는다면 시작지점
//// 못찾는다면 nullptr
char* telnet_parser::find_enter_from_payload(char* in_payload, size_t in_size)
{

	for (size_t n{}; n < in_size - 1; ++n)
	{
		uint16_t* in_buf = reinterpret_cast<uint16_t*>(&in_payload[n]);
		//if (in_payload[n] == '\r\n')
		if (*in_buf == '\r\n')
		{
			return &in_payload[n];
		}
	}

	return nullptr;
}
//



size_t telnet_parser::login_handler(chat_session* sess, char* in_buffer, size_t size)
{
	// in_buffer는 0이 아니라고 가정.
	// (sess + in_buffer) != in_buffer 이라면 sess는 nullptr이 아니다.
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// 중복 로그인 처리...
	if (e_session_state::LOGINED <= sess->get_state())
	{
		chat_user* user = (chat_user*)sess->get_user(); crash_if_false(nullptr);

		LOG("이미 로그인한 유저 %s님이 재 로그인 시도중입니다.\r\n", user->get_name().c_str());

		return 0;
	}


	// space 넘기
	char* id_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = telnet_parser::find_enter_from_payload(id_str, size);
	if (nullptr == end_line_str) // 잘못된 명령어 처리.
	{
		sess->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));

		return 0;
	}


	size_t name_size = end_line_str - id_str;
	string user_name {id_str, name_size};

	// user 할당.
	chat_server* server = reinterpret_cast<chat_server*>(sess->get_server());
	crash_if_false(nullptr != server);

	chat_user* user = reinterpret_cast<chat_user*>(server->allocate_user(user_name));
	crash_if_false(nullptr != user);

	sess->set_user(user);
	user->set_session(sess);
	user->enter_lobby();

	printf("%s 유저가 로그인 했습니다.\r\n ", user->get_name().c_str());

	// 로그인의 경우 space(1) + id_size + '\r\n';
	return 1 + name_size + 2;
}

size_t telnet_parser::create_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// space 넘기 + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = telnet_parser::find_enter_from_payload(room_name_str, size);
	if (nullptr == end_line_str) // 명령어 완성 X 
	{
		return 0;
	}

	// room name 정보.
	size_t	room_name_size = end_line_str - room_name_str;
	string	room_name{ room_name_str, room_name_size };


	// user lobby 접속 처리.
	chat_room* room = g_room_manager->allocate(room_name);
	if (nullptr == room) // room 64개가 전부 할당 된 경우...
	{
		// 안내 메시지 출력.
		printf("방 생성을 더 이상 할 수 없습니다.\r\n", room_name.c_str() );
		
		sess->pre_send(room_full_msg, sizeof(room_full_msg));
	}
	else // 방 접속 가능시...
	{
		room->set_name(std::move(room_name));

		chat_user* user = (chat_user*)(sess->get_user());

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// 안내 메시지 출력
		sess->pre_send(enter_room_msg, sizeof(enter_room_msg));
	}
	
	// 방생성의 경우 space(1) + id_size + '\r\n';
	return 1 + room_name_size + 2;
}

size_t telnet_parser::enter_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// space 넘기 + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = telnet_parser::find_enter_from_payload(room_name_str, size);
	if (nullptr == end_line_str) // 명령어 완성 X 
	{
		return 0;
	}

	// room name 정보.
	size_t	room_name_size = end_line_str - room_name_str;
	string	room_name{ room_name_str, room_name_size };


	// 방이름을 통해 찾기.
	chat_room* room = g_room_manager->find_room_using_name(room_name);
	if (nullptr == room) // 찾는 방이 없는 경우.
	{
		sess->pre_send(not_find_room_msg, sizeof(not_find_room_msg)); // 안내 메시지 출력.
	}
	else // 방 접속 가능시...
	{
		chat_user* user = (chat_user*)sess->get_user();

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// 안내 메시지 출력
		sess->pre_send(enter_room_msg, sizeof(enter_room_msg));
	}

	// 방 접속의 경우 space(1) + room_name_size + '\r\n';
	return 1 + room_name_size + 2;
}

size_t telnet_parser::leave_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // 명령어 완성 X 
	{
		return 0;
	}

	// user를 구하고...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	
	if (e_user_state::US_IN_ROOM != user->get_state()) // 내가 방에 없는 경우...
	{
		sess->pre_send(not_in_room_msg, sizeof(not_in_room_msg)); // 안내 메시지 출력.
	}
	else // 내가 방에 있는 경우...
	{
		sess->pre_send(leave_user_from_room_msg, sizeof(leave_user_from_room_msg)); // 안내 메시지 출력.
		user->leave_room();
	}
	
	// 방 떠나기의 경우 '\r\n'(2byte);
	return 0;
}



size_t telnet_parser::select_rooms_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // 명령어 완성 X 
	{
		return 0;
	}

	// user를 구하고...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// 방 리스트...
	string&& active_room_list = g_room_manager->get_active_room_list_to_string();
	if (active_room_list == "")
	{
		active_room_list = "현재 생성된 방이 없습니다.\r\n";
	}


	sess->pre_send(active_room_list.c_str(), active_room_list.size()); // 안내 메시지 출력.

	// 방 떠나기의 경우 '\r\n'(2byte);
	return 2;
}

size_t telnet_parser::select_user_in_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // 명령어 완성 X 
	{
		return 0;
	}


	// user를 구하고...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);
	chat_room* room = (chat_room*)user->get_room();
	crash_if_false(nullptr != room);
	

	if (e_user_state::US_IN_ROOM != user->get_state()) // 내가 방에 없는 경우...
	{
		sess->pre_send(not_find_room_msg, sizeof(not_find_room_msg)); // 안내 메시지 출력.
	}
	else // 내가 방에 있는 경우...
	{
		// 방 리스트...
		string user_list_in_room = room->get_user_list_in_room_to_string();
		if (user_list_in_room == "")
		{
			user_list_in_room = "현재 방에 유저가 없습니다.\r\n";
		}

		sess->pre_send(user_list_in_room.c_str(), user_list_in_room.size()); // 안내 메시지 출력.
	}


	// 방속 유저 목록 조회의 경우 '\r\n'(2byte);
	return 2;
}

size_t telnet_parser::select_all_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // 명령어 완성 X 
	{
		return 0;
	}

	//char carage_return = in_buffer[1];
	//if ('\r' != carage_return)
	//{
	//	return 0;
	//}

	chat_server* server = (chat_server*)sess->get_server();
	crash_if_false(nullptr != server);


	string all_user_list = server->get_active_user_to_string();
	if (all_user_list == "")
	{
		// 나까지 로그인 안한 상태...
		all_user_list = "현재 유저가 없습니다.\r\n";
	}

	sess->pre_send(all_user_list.c_str(), all_user_list.size()); // 안내 메시지 출력.


	return 2;
}



size_t telnet_parser::whisper_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// user를 구하고
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// space 넘기 + 1byte
	char* target_name_str = &in_buffer[1];
	size -= 1;



	//명령어 끝
	char* end_line_str = telnet_parser::find_enter_from_payload(target_name_str, size);
	if (nullptr == end_line_str) // 이상한 명령어
	{
		crash();
		printf("유효 X 명령어 \r\n");
		return 0;
	}


	// 다음 space
	char* next_space_str = telnet_parser::find_space_from_payload(target_name_str, size);
	if (nullptr == next_space_str) // 유효하지 않은 명령어...  
	{
		sess->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));

		return 0;
	}


	// string target name 만들기.
	size_t target_name_size = next_space_str - target_name_str;
	string target_name{target_name_str, target_name_size};
	
	chat_server* server = (chat_server*)sess->get_server();
	crash_if_false(nullptr != server);

	chat_user* target_user = server->find_user_using_name(target_name);
	if (nullptr == target_user) // 찾는 유저가 없다면?
	{
		LOG("%s님이 찾는 유저 %s님이 없습니다.", user->get_name().c_str(), target_name.c_str());
		// 찾는 상대 유저가 없다는것을 알림.
		sess->pre_send(not_find_user_msg, sizeof(not_find_user_msg));

		return 0;
	}

	chat_session* target_session = (chat_session*)(target_user->get_session());
	crash_if_false(nullptr != target_session);

	//target_session->parse_send();
	// space 다음 위치
	next_space_str += 1;
	size -= (target_name_size + 1);


	// space 넘기 + 1byte
	char* whisper_msg_str = next_space_str;
	size_t whisper_msg_size = end_line_str - next_space_str;
	string whisper_msg{next_space_str, whisper_msg_size};

	whisper_msg = user->get_name() + "님의 귓속말 : " + whisper_msg;
	whisper_msg += "\r\n";
	whisper_msg += end_msg;

	// 전송 요청.
	target_session->pre_send(whisper_msg.c_str(), whisper_msg.size());

	return 2;
}

size_t telnet_parser::chatting_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	//명령어 끝
	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // 명령어 완성 X 
	{
		crash();
		printf("유효 X 명령어 \r\n");
		return 0;
	}

	char*  msg_str = in_buffer;
	if (' ' != *msg_str && '\r' != *msg_str)
	{
		return 0;
	}

	msg_str = &in_buffer[1];
	if ((size_t)msg_str > (size_t)end_line_str)
	{
		sess->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));
		return 0;
	}

	size_t msg_size = end_line_str - msg_str;
	string msg{ msg_str, msg_size };


	// 로그인 전 user가 없음.

	// user lobby 처리.
	chat_user* user = reinterpret_cast<chat_user*>(sess->get_user());
	if (nullptr == user)
	{
		sess->pre_send(before_login_msg, sizeof(before_login_msg));
		return 0;
	}

	msg = user->get_name() + " : " + msg + "\r\n";

	switch (user->get_state())
	{
		case e_user_state::US_NONE:
		{
			sess->pre_send(before_login_msg, sizeof(before_login_msg));
			break;
		}

		case e_user_state::US_IN_LOBBY: 
		{
			g_lobby->broadcast((char*)msg.c_str(), msg.size());
			break;
		}
		case e_user_state::US_IN_ROOM:
		{
			chat_room* room = user->get_room();
			crash_if_false(nullptr != room);

			room->broadcast((char*)msg.c_str(), msg.size());
		}
		break;

		/*case e_user_state::US_NONE:
		{
			sess->pre_send(already_closed_msg, sizeof(already_closed_msg));
			break;
		}
		*/


	default:
		break;
	}

	// 로그인의 경우 space(1) + id_size + '\r\n';
	return 1 + msg_size + 2;
}
