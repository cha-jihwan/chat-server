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
		// 남은 사이즈가 최소 명령 사이즈 보다 작다면 종료.
		// 4 = commnand_size (2) + \r\n (2)
		// body_next 즉 [body][\r\n] 중 \r\n을 찾고 \r부분을 반환한다.
		// 최소 2바이트 이상 있다는 뜻.
		char* cmd_start = &in_buffer[parsed_size];
		char* carage_return = telnet_parser::find_enter_from_payload(cmd_start, in_size - parsed_size);
		if (nullptr == carage_return) // 명령어 완성 X
		{
			//in_session->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));

			// 명령어가 완선 X parsed_size 변경 없이 나감.

			break;
		}


		// 로깅용 무조건 carage_return + c_cmd_size가 cmd_str보다 크다 라는 가정...
		// \r\n 이전 사이즈.
		size_t current_cmd_size{ (size_t)(carage_return) - (size_t)cmd_start };
		if ((size_t)(carage_return) < (size_t)cmd_start)
		{
			LOG("( current_cmd_size < 0 )\n");
			crash();
		}


		// find 이상한 문자 뛰어 넘기 '/'
		// cmd_first_char  ==  /
		char* cmd_first_char = telnet_parser::find_character_from_payload(cmd_start, current_cmd_size, '/');
		
		current_cmd_size += c_crlf_size;

		if (nullptr == cmd_first_char)
		{
			in_session->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());

			parsed_size += current_cmd_size;

			break;
		}


		// 명령어 사이즈 체크.
		int64_t cmd_size = (size_t)carage_return - (size_t)cmd_first_char;
		if (2 >cmd_size) // 최소 case 1이상은 되야 하니
		{
			in_session->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());
			
			parsed_size += current_cmd_size;

			break;
		}

		string cmd{cmd_first_char, c_cmd_size};
		int64_t body_and_option_size = carage_return - (cmd_first_char+c_cmd_size); // /c  cmd 이후 \r\n 이전 남은 사이즈
		if (body_and_option_size < 0)
		{
			LOG("( body_and_option_size < 0 )\n");
			crash();
		}


		// cmd 이후 \r\n 이전 부분을 handler에서 처리..
		size_t returned_handling_size{};
		if (1 <= packet_handlers.count(cmd))
		{
			returned_handling_size = packet_handlers[cmd](in_session, &cmd_first_char[c_cmd_size], body_and_option_size);
		}
		else
		{
			in_session->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());
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


bool telnet_parser::is_logined(chat_session* sess, char* fmt)
{
	crash_if_false(nullptr != sess);

	if (e_session_state::LOGINED > sess->get_state())
	{
		LOG(fmt);

		sess->pre_send(request_login_msg.c_str(), request_login_msg.size());

		return false;
	}

	return true;
}

//
//// payload로 부터 인자로 들어오 ch 문자를 찾는다.
//// return value 
//// 문자열을 찾는다면 시작지점
//// 못찾는다면 nullptr
//// 문자를 찾지 못한다면 nullptr
//// 문자를 찾는다면 위치 시작 지점.
char* telnet_parser::find_character_from_payload(char * in_buf, size_t in_size, char ch)
{
	if (in_size  == 0)
	{
		return nullptr;
	}

	for (size_t n{}; n < in_size; ++n)
	{
		if (in_buf[n] == ch)
		{
			return &in_buf[n];
		}
	}

	return nullptr;
}

char* telnet_parser::pass_invalid_character_from_payload(char* in_buf, size_t in_size)
{
	if (in_size == 0)
	{
		return nullptr;
	}

	for (size_t n{}; n < in_size; ++n)
	{
		switch (in_buf[n])
		{
			case '\r': continue;
			case '\n': continue;
			case ' ' : continue;
				

			default:
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
	if (in_size == 0)
	{
		return nullptr;
	}

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
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// 중복 로그인 처리...
	if (sess->is_logined())
	{
		chat_user* user = (chat_user*)sess->get_user(); 
		
		crash_if_false(nullptr != user);

		LOG("로그인 실패 이미 로그인한 유저 %s님이 재 로그인 시도중입니다.\r\n", user->get_name().c_str());

		return 0;
	}

	// 1이면 공백만 있는 것.
	if (size <= 1)
	{
		sess->pre_send(request_id_msg.c_str(), request_id_msg.size());

		return 0;
	}

	// space 넘기
	char* id_str = &in_buffer[1];
	size -= 1;

	size_t name_size = (size_t)&id_str[size] - (size_t)id_str;
	string user_name {id_str, (size_t)name_size};
	if (string::npos != user_name.find(' ')) // 중간에 공백 포함된 경우.
	{
		sess->pre_send(blank_id_msg.c_str(), blank_id_msg.size());

		return 0;
	}

	// user 할당.
	chat_server* server = reinterpret_cast<chat_server*>(sess->get_server());
	crash_if_false(nullptr != server);

	chat_user* user = reinterpret_cast<chat_user*>(server->allocate_user(user_name));
	crash_if_false(nullptr != user);

	user->set_session(sess);
	user->enter_lobby();
	sess->set_user(user);
	sess->set_state(e_session_state::LOGINED);

	printf("%s 유저가 로그인 했습니다.\r\n ", user->get_name().c_str());

	sess->pre_send(user->get_name().c_str(), user->get_name().size());
	sess->pre_send(login_msg.c_str(), login_msg.size());

	return 0;
}

size_t telnet_parser::create_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);


	if (false == is_logined(sess, "룸 생성 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}


	// space 넘기 + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	// room name 정보.
	size_t	room_name_size = (size_t)&room_name_str[size] - (size_t)room_name_str;
	string	room_name{ room_name_str, room_name_size };

	//// user_name 중가에 space 들어가면
	//if (string::npos != room_name.find(' '))
	//{
	//	sess->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));
	//
	//	return 0;
	//}

	// user lobby 접속 처리.
	chat_room* room = g_room_manager->allocate(room_name);
	if (nullptr == room) // room 64개가 전부 할당 된 경우...
	{
		// 안내 메시지 출력.
		printf("방 생성을 더 이상 할 수 없습니다.\r\n", room_name.c_str() );
		
		sess->pre_send(room_full_msg.c_str(), room_full_msg.size());
	}
	else // 방 접속 가능시...
	{
		room->set_name(std::move(room_name));

		chat_user* user = (chat_user*)(sess->get_user());

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// 안내 메시지 출력
		sess->pre_send(create_room_msg.c_str(), create_room_msg.size());
	}
	
	return 0;
}

size_t telnet_parser::enter_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "룸 접속 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}

	// space 넘기 + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = &in_buffer[size];

	// room name 정보.
	size_t	room_name_size = size;
	string	room_name{ room_name_str, room_name_size };

	// 방이름을 통해 찾기.
	chat_room* room = g_room_manager->find_room_using_name(room_name);
	if (nullptr == room) // 찾는 방이 없는 경우.
	{
		sess->pre_send(not_find_room_msg.c_str(), not_find_room_msg.size()); // 안내 메시지 출력.
	}
	else // 방 접속 가능시...
	{
		chat_user* user = (chat_user*)sess->get_user();

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// 안내 메시지 출력
		sess->pre_send(enter_room_msg.c_str(), enter_room_msg.size());
	}

	return 0;
}

size_t telnet_parser::leave_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "룸 탈출 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}


	// user 획득
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);


	if (e_user_state::US_IN_ROOM != user->get_state()) // 내가 방에 없는 경우...
	{
		sess->pre_send(not_in_room_msg.c_str(), not_in_room_msg.size()); // 안내 메시지 출력.
	}
	else // 내가 방에 있는 경우...
	{
		sess->pre_send(leave_user_from_room_msg.c_str(), leave_user_from_room_msg.size()); // 안내 메시지 출력.
		user->leave_room();
	}
	
	return 0;
}



size_t telnet_parser::select_rooms_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "룸 조회 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}

	// user를 구하고...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// 방 리스트...
	string&& active_room_list = g_room_manager->get_active_room_list_to_string();
	if (active_room_list == packet_filter_keywords[(size_t)e_packet_filter::EPF_SELECT_ROOM_LIST])
	{
		active_room_list += "현재 생성된 방이 없습니다.\r\n";
	}

	sess->pre_send(active_room_list.c_str(), active_room_list.size()); // 안내 메시지 출력.

	// 방 떠나기의 경우 '\r\n'(2byte);
	return 2;
}





size_t telnet_parser::whisper_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "귓속말 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}

	// user를 구하고
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// space 넘기
	char* target_name_str = telnet_parser::pass_invalid_character_from_payload(in_buffer, size);
	size_t buf_size = target_name_str - in_buffer;
	if(nullptr == target_name_str)
	{
		LOG("target_name == nullptr\n");

		sess->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());

		return 0;
	}


	if (size < buf_size)
	{
		LOG("size < buf_size\n");

		return 0;
	}

	// 다음 space 찾기
	char* target_name_end = telnet_parser::find_character_from_payload(target_name_str, size - buf_size, ' ');
	if (nullptr == target_name_end) // 이상한 명령어
	{
		LOG("target_name == nullptr\n");

		sess->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());

		return 0;
	}


	// string target name 만들기.
	size_t target_name_size = target_name_end - target_name_str;
	string target_name{target_name_str, target_name_size};
	
	chat_server* server = (chat_server*)sess->get_server();
	crash_if_false(nullptr != server);

	chat_user* target_user = server->find_user_using_name(target_name);
	if (nullptr == target_user) // 찾는 유저가 없다면?
	{
		LOG("%s님이 찾는 유저 %s님이 없습니다.", user->get_name().c_str(), target_name.c_str());
		// 찾는 상대 유저가 없다는것을 알림.
		sess->pre_send(not_find_user_msg.c_str(), not_find_user_msg.size());

		return 0;
	}

	chat_session* target_session = (chat_session*)(target_user->get_session());
	crash_if_false(nullptr != target_session);

	//target_session->parse_send();
	// space 다음 위치



	// space 넘기
	buf_size = size - (target_name_end - in_buffer);
	char* msg_str = telnet_parser::pass_invalid_character_from_payload(target_name_end, buf_size);
	if (nullptr == msg_str)
	{
		LOG("msg_str == nullptr\n");

		sess->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());

		return 0;
	}


	// space 넘기 + 1byte
	string whisper_msg{msg_str, buf_size};

	whisper_msg = user->get_name() + "님의 귓속말 : " + whisper_msg;
	whisper_msg += "\r\n";
	whisper_msg += end_msg;

	// 전송 요청.
	target_session->pre_send(whisper_msg.c_str(), whisper_msg.size());

	return 0;
}

size_t telnet_parser::chatting_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "채팅 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}

	//명령어 끝
	char* slash_r_str = &in_buffer[size];
	char* msg_str = in_buffer;
	if (' ' != *msg_str && '\r' != *msg_str)
	{
		sess->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());
		return 0;
	}

	msg_str = &in_buffer[1];
	if ((size_t)msg_str > (size_t)slash_r_str)
	{
		sess->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());
		return 0;
	}

	size_t msg_size = slash_r_str - msg_str;
	string msg{msg_str, msg_size};


	// user lobby 처리.
	chat_user* user = reinterpret_cast<chat_user*>(sess->get_user());
	if (nullptr == user)
	{
		sess->pre_send(before_login_msg.c_str(), before_login_msg.size());
		return 0;
	}

	msg = user->get_name() + "님의 채팅 : " + msg + "\r\n";

	switch (user->get_state())
	{
		case e_user_state::US_NONE:
		{
			sess->pre_send(before_login_msg.c_str(), before_login_msg.size());
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
	return 0;
}

////////////////////////////////////////////////////////////////////
// case 1
////////////////////////////////////////////////////////////////////
size_t telnet_parser::select_user_in_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "룸내의 유저 조회 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}


	if (size > 0) // 특수 처리...
 	{
		
	}

	// user를 구하고...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);
	chat_room* room = (chat_room*)(user->get_room());
	crash_if_false(nullptr != room);

	if (e_user_state::US_IN_ROOM != user->get_state()) // 내가 방에 없는 경우...
	{
		sess->pre_send(not_in_room_msg.c_str(), not_in_room_msg.size()); // 안내 메시지 출력.
	}
	else // 내가 방에 있는 경우...
	{
		// 방 리스트를 string객체로 획득.
		string user_list_in_room = room->get_user_list_in_room_to_string(); // 
		if (user_list_in_room == "")
		{
			user_list_in_room = "현재 방에 유저가 없습니다.\r\n";
		}
		user_list_in_room += "\0\0";
		sess->pre_send(user_list_in_room.c_str(), user_list_in_room.size()); // 안내 메시지 출력.
	}

	return 0;
}



size_t telnet_parser::select_all_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "총 유저 조회 실패 사유 : 비로그인 유저 \n"))
	{
		return 0;
	}

	chat_server* server = (chat_server*)sess->get_server();
	crash_if_false(nullptr != server);

	string all_user_list = server->get_active_user_to_string();
	if (all_user_list == "전체 유저 목록") // 나 하나쯤은 있겠지... 당연히..
	{
		// 나까지 로그인 안한 상태...
		all_user_list = "현재 유저가 없습니다.";
	}

	sess->pre_send(all_user_list.c_str(), all_user_list.size()); // 안내 메시지 출력.

	return 2;
}