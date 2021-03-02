#include "../common/pre_compile.h"

unordered_map<std::string, telnet_parser::packet_handler> telnet_parser::packet_handlers;

// �ּ� ��ɾ� ũ�� case 1: 1 + N + 2
// case 1:'\'(1) + cmd(���� N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
// case 2:'\'(1) + cmd(���� N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
// case 3:'\'(1) + cmd(���� N) + 1(sapce) + ���ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
// case 1: command\r\n
// case 2: command space body \r\n
// case 3: command space target_name body \r\n
size_t telnet_parser::parse_payload(c2::server::contents::chat_session* in_session, char* in_buffer, size_t in_size, std::vector<packet>& out_buffer)
{
	size_t	parsed_size{};

	for (;;)
	{
		// ���� ����� �ּ� ��� ������ ���� �۴ٸ� ����.
		// 4 = commnand_size (2) + \r\n (2)
		// body_next �� [body][\r\n] �� \r\n�� ã�� \r�κ��� ��ȯ�Ѵ�.
		// �ּ� 2����Ʈ �̻� �ִٴ� ��.
		char* cmd_start = &in_buffer[parsed_size];
		char* carage_return = telnet_parser::find_enter_from_payload(cmd_start, in_size - parsed_size);
		if (nullptr == carage_return) // ��ɾ� �ϼ� X
		{
			//in_session->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));

			// ��ɾ �ϼ� X parsed_size ���� ���� ����.

			break;
		}


		// �α�� ������ carage_return + c_cmd_size�� cmd_str���� ũ�� ��� ����...
		// \r\n ���� ������.
		size_t current_cmd_size{ (size_t)(carage_return) - (size_t)cmd_start };
		if ((size_t)(carage_return) < (size_t)cmd_start)
		{
			LOG("( current_cmd_size < 0 )\n");
			crash();
		}


		// find �̻��� ���� �پ� �ѱ� '/'
		// cmd_first_char  ==  /
		char* cmd_first_char = telnet_parser::find_character_from_payload(cmd_start, current_cmd_size, '/');
		
		current_cmd_size += c_crlf_size;

		if (nullptr == cmd_first_char)
		{
			in_session->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());

			parsed_size += current_cmd_size;

			break;
		}


		// ��ɾ� ������ üũ.
		int64_t cmd_size = (size_t)carage_return - (size_t)cmd_first_char;
		if (2 >cmd_size) // �ּ� case 1�̻��� �Ǿ� �ϴ�
		{
			in_session->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());
			
			parsed_size += current_cmd_size;

			break;
		}

		string cmd{cmd_first_char, c_cmd_size};
		int64_t body_and_option_size = carage_return - (cmd_first_char+c_cmd_size); // /c  cmd ���� \r\n ���� ���� ������
		if (body_and_option_size < 0)
		{
			LOG("( body_and_option_size < 0 )\n");
			crash();
		}


		// cmd ���� \r\n ���� �κ��� handler���� ó��..
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
	//// �α���
	packet_handlers.emplace("/L", telnet_parser::login_handler);
	packet_handlers.emplace("/l", telnet_parser::login_handler);

	//// �� ����
	packet_handlers.emplace("/R", telnet_parser::create_room_handler);
	packet_handlers.emplace("/r", telnet_parser::create_room_handler);

	//// �� ����
	packet_handlers.emplace("/Q", telnet_parser::enter_room_handler);
	packet_handlers.emplace("/q", telnet_parser::enter_room_handler);

	//// �� ������
	packet_handlers.emplace("/E", telnet_parser::leave_room_handler);
	packet_handlers.emplace("/e", telnet_parser::leave_room_handler);

	//// ä��
	packet_handlers.emplace("/C", telnet_parser::chatting_handler);
	packet_handlers.emplace("/c", telnet_parser::chatting_handler);

	//// �� ���
	packet_handlers.emplace("/S", telnet_parser::select_rooms_handler);
	packet_handlers.emplace("/s", telnet_parser::select_rooms_handler);

	//// �ӼӸ�
	packet_handlers.emplace("/W", telnet_parser::whisper_handler);
	packet_handlers.emplace("/w", telnet_parser::whisper_handler);

	//// �� ������ ���
	packet_handlers.emplace("/U", telnet_parser::select_user_in_room_handler);
	packet_handlers.emplace("/u", telnet_parser::select_user_in_room_handler);

	//// ������ ���
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
//// payload�� ���� ���ڷ� ���� ch ���ڸ� ã�´�.
//// return value 
//// ���ڿ��� ã�´ٸ� ��������
//// ��ã�´ٸ� nullptr
//// ���ڸ� ã�� ���Ѵٸ� nullptr
//// ���ڸ� ã�´ٸ� ��ġ ���� ����.
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





//// payload�� ���� telnet�� enter�Է� (linfeed + carage-return)�� ã�´�.
//// \r\n 
//// return value 
//// ���ڿ��� ã�´ٸ� ��������
//// ��ã�´ٸ� nullptr
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

	// �ߺ� �α��� ó��...
	if (sess->is_logined())
	{
		chat_user* user = (chat_user*)sess->get_user(); 
		
		crash_if_false(nullptr != user);

		LOG("�α��� ���� �̹� �α����� ���� %s���� �� �α��� �õ����Դϴ�.\r\n", user->get_name().c_str());

		return 0;
	}

	// 1�̸� ���鸸 �ִ� ��.
	if (size <= 1)
	{
		sess->pre_send(request_id_msg.c_str(), request_id_msg.size());

		return 0;
	}

	// space �ѱ�
	char* id_str = &in_buffer[1];
	size -= 1;

	size_t name_size = (size_t)&id_str[size] - (size_t)id_str;
	string user_name {id_str, (size_t)name_size};
	if (string::npos != user_name.find(' ')) // �߰��� ���� ���Ե� ���.
	{
		sess->pre_send(blank_id_msg.c_str(), blank_id_msg.size());

		return 0;
	}

	// user �Ҵ�.
	chat_server* server = reinterpret_cast<chat_server*>(sess->get_server());
	crash_if_false(nullptr != server);

	chat_user* user = reinterpret_cast<chat_user*>(server->allocate_user(user_name));
	crash_if_false(nullptr != user);

	user->set_session(sess);
	user->enter_lobby();
	sess->set_user(user);
	sess->set_state(e_session_state::LOGINED);

	printf("%s ������ �α��� �߽��ϴ�.\r\n ", user->get_name().c_str());

	sess->pre_send(user->get_name().c_str(), user->get_name().size());
	sess->pre_send(login_msg.c_str(), login_msg.size());

	return 0;
}

size_t telnet_parser::create_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);


	if (false == is_logined(sess, "�� ���� ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}


	// space �ѱ� + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	// room name ����.
	size_t	room_name_size = (size_t)&room_name_str[size] - (size_t)room_name_str;
	string	room_name{ room_name_str, room_name_size };

	//// user_name �߰��� space ����
	//if (string::npos != room_name.find(' '))
	//{
	//	sess->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));
	//
	//	return 0;
	//}

	// user lobby ���� ó��.
	chat_room* room = g_room_manager->allocate(room_name);
	if (nullptr == room) // room 64���� ���� �Ҵ� �� ���...
	{
		// �ȳ� �޽��� ���.
		printf("�� ������ �� �̻� �� �� �����ϴ�.\r\n", room_name.c_str() );
		
		sess->pre_send(room_full_msg.c_str(), room_full_msg.size());
	}
	else // �� ���� ���ɽ�...
	{
		room->set_name(std::move(room_name));

		chat_user* user = (chat_user*)(sess->get_user());

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// �ȳ� �޽��� ���
		sess->pre_send(create_room_msg.c_str(), create_room_msg.size());
	}
	
	return 0;
}

size_t telnet_parser::enter_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "�� ���� ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}

	// space �ѱ� + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = &in_buffer[size];

	// room name ����.
	size_t	room_name_size = size;
	string	room_name{ room_name_str, room_name_size };

	// ���̸��� ���� ã��.
	chat_room* room = g_room_manager->find_room_using_name(room_name);
	if (nullptr == room) // ã�� ���� ���� ���.
	{
		sess->pre_send(not_find_room_msg.c_str(), not_find_room_msg.size()); // �ȳ� �޽��� ���.
	}
	else // �� ���� ���ɽ�...
	{
		chat_user* user = (chat_user*)sess->get_user();

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// �ȳ� �޽��� ���
		sess->pre_send(enter_room_msg.c_str(), enter_room_msg.size());
	}

	return 0;
}

size_t telnet_parser::leave_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "�� Ż�� ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}


	// user ȹ��
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);


	if (e_user_state::US_IN_ROOM != user->get_state()) // ���� �濡 ���� ���...
	{
		sess->pre_send(not_in_room_msg.c_str(), not_in_room_msg.size()); // �ȳ� �޽��� ���.
	}
	else // ���� �濡 �ִ� ���...
	{
		sess->pre_send(leave_user_from_room_msg.c_str(), leave_user_from_room_msg.size()); // �ȳ� �޽��� ���.
		user->leave_room();
	}
	
	return 0;
}



size_t telnet_parser::select_rooms_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "�� ��ȸ ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}

	// user�� ���ϰ�...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// �� ����Ʈ...
	string&& active_room_list = g_room_manager->get_active_room_list_to_string();
	if (active_room_list == packet_filter_keywords[(size_t)e_packet_filter::EPF_SELECT_ROOM_LIST])
	{
		active_room_list += "���� ������ ���� �����ϴ�.\r\n";
	}

	sess->pre_send(active_room_list.c_str(), active_room_list.size()); // �ȳ� �޽��� ���.

	// �� �������� ��� '\r\n'(2byte);
	return 2;
}





size_t telnet_parser::whisper_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "�ӼӸ� ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}

	// user�� ���ϰ�
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// space �ѱ�
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

	// ���� space ã��
	char* target_name_end = telnet_parser::find_character_from_payload(target_name_str, size - buf_size, ' ');
	if (nullptr == target_name_end) // �̻��� ��ɾ�
	{
		LOG("target_name == nullptr\n");

		sess->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());

		return 0;
	}


	// string target name �����.
	size_t target_name_size = target_name_end - target_name_str;
	string target_name{target_name_str, target_name_size};
	
	chat_server* server = (chat_server*)sess->get_server();
	crash_if_false(nullptr != server);

	chat_user* target_user = server->find_user_using_name(target_name);
	if (nullptr == target_user) // ã�� ������ ���ٸ�?
	{
		LOG("%s���� ã�� ���� %s���� �����ϴ�.", user->get_name().c_str(), target_name.c_str());
		// ã�� ��� ������ ���ٴ°��� �˸�.
		sess->pre_send(not_find_user_msg.c_str(), not_find_user_msg.size());

		return 0;
	}

	chat_session* target_session = (chat_session*)(target_user->get_session());
	crash_if_false(nullptr != target_session);

	//target_session->parse_send();
	// space ���� ��ġ



	// space �ѱ�
	buf_size = size - (target_name_end - in_buffer);
	char* msg_str = telnet_parser::pass_invalid_character_from_payload(target_name_end, buf_size);
	if (nullptr == msg_str)
	{
		LOG("msg_str == nullptr\n");

		sess->pre_send(invalid_cmd_msg.c_str(), invalid_cmd_msg.size());

		return 0;
	}


	// space �ѱ� + 1byte
	string whisper_msg{msg_str, buf_size};

	whisper_msg = user->get_name() + "���� �ӼӸ� : " + whisper_msg;
	whisper_msg += "\r\n";
	whisper_msg += end_msg;

	// ���� ��û.
	target_session->pre_send(whisper_msg.c_str(), whisper_msg.size());

	return 0;
}

size_t telnet_parser::chatting_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "ä�� ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}

	//��ɾ� ��
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


	// user lobby ó��.
	chat_user* user = reinterpret_cast<chat_user*>(sess->get_user());
	if (nullptr == user)
	{
		sess->pre_send(before_login_msg.c_str(), before_login_msg.size());
		return 0;
	}

	msg = user->get_name() + "���� ä�� : " + msg + "\r\n";

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

	// �α����� ��� space(1) + id_size + '\r\n';
	return 0;
}

////////////////////////////////////////////////////////////////////
// case 1
////////////////////////////////////////////////////////////////////
size_t telnet_parser::select_user_in_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "�볻�� ���� ��ȸ ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}


	if (size > 0) // Ư�� ó��...
 	{
		
	}

	// user�� ���ϰ�...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);
	chat_room* room = (chat_room*)(user->get_room());
	crash_if_false(nullptr != room);

	if (e_user_state::US_IN_ROOM != user->get_state()) // ���� �濡 ���� ���...
	{
		sess->pre_send(not_in_room_msg.c_str(), not_in_room_msg.size()); // �ȳ� �޽��� ���.
	}
	else // ���� �濡 �ִ� ���...
	{
		// �� ����Ʈ�� string��ü�� ȹ��.
		string user_list_in_room = room->get_user_list_in_room_to_string(); // 
		if (user_list_in_room == "")
		{
			user_list_in_room = "���� �濡 ������ �����ϴ�.\r\n";
		}
		user_list_in_room += "\0\0";
		sess->pre_send(user_list_in_room.c_str(), user_list_in_room.size()); // �ȳ� �޽��� ���.
	}

	return 0;
}



size_t telnet_parser::select_all_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	if (false == is_logined(sess, "�� ���� ��ȸ ���� ���� : ��α��� ���� \n"))
	{
		return 0;
	}

	chat_server* server = (chat_server*)sess->get_server();
	crash_if_false(nullptr != server);

	string all_user_list = server->get_active_user_to_string();
	if (all_user_list == "��ü ���� ���") // �� �ϳ����� �ְ���... �翬��..
	{
		// ������ �α��� ���� ����...
		all_user_list = "���� ������ �����ϴ�.";
	}

	sess->pre_send(all_user_list.c_str(), all_user_list.size()); // �ȳ� �޽��� ���.

	return 2;
}