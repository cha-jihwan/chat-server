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
		char* buf_ptr = &in_buffer[parsed_size];
		// ���� ����� �ּ� ��� ������ ���� �۴ٸ� ����.
		// 4 = commnand_size (2) + \r\n (2)
		if (4 > in_size - parsed_size)
		{
			break;
		}

		char* buf_end = telnet_parser::find_enter_from_payload(buf_ptr, in_size - parsed_size);
		if (nullptr == buf_end) // ��ɾ� �ϼ� X
		{
			break;
		}

		size_t current_cmd_size{(size_t)(buf_end + 2) - (size_t)buf_ptr};
		size_t return_size{};
		string cmd{buf_ptr, 2};

		// 1���� ���� ���...
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

//
//// payload�� ���� ���ڸ� ã�´�.
//// return value 
//// ���ڿ��� ã�´ٸ� ��������
//// ��ã�´ٸ� nullptr
//// ���ڸ� ã�� ���Ѵٸ� nullptr
//// ���ڸ� ã�´ٸ� ��ġ ���� ����.
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





//// payload�� ���� telnet�� enter�Է� (linfeed + carage-return)�� ã�´�.
//// \r\n 
//// return value 
//// ���ڿ��� ã�´ٸ� ��������
//// ��ã�´ٸ� nullptr
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
	// sess�� 0�� �ƴ϶�� ����.
	// (sess + in_buffer) != in_buffer �̶�� sess�� nullptr�� �ƴϴ�.
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);
	
	// space �ѱ�
	char* id_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = telnet_parser::find_enter_from_payload(id_str, size);
	if (nullptr == end_line_str)
	{
		return 0;
	}

	size_t name_size = end_line_str - id_str;
	string user_name {id_str, name_size};

	// user �Ҵ�.
	chat_server* server = reinterpret_cast<chat_server*>(sess->get_server());
	crash_if_false(nullptr != server);
	chat_user* user = reinterpret_cast<chat_user*>(server->allocate_user(user_name));
	crash_if_false(nullptr != user);

	sess->set_user(user);
	user->set_session(sess);
	user->enter_lobby();

	printf("%s ������ �α��� �߽��ϴ�.\r\n ", user->get_name().c_str());

	// �α����� ��� space(1) + id_size + '\r\n';
	return 1 + name_size + 2;
}

size_t telnet_parser::create_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// space �ѱ� + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = telnet_parser::find_enter_from_payload(room_name_str, size);
	if (nullptr == end_line_str) // ��ɾ� �ϼ� X 
	{
		return 0;
	}

	// room name ����.
	size_t	room_name_size = end_line_str - room_name_str;
	string	room_name{ room_name_str, room_name_size };


	// user lobby ���� ó��.
	chat_room* room = g_room_manager->allocate(room_name);
	if (nullptr == room) // room 64���� ���� �Ҵ� �� ���...
	{
		// �ȳ� �޽��� ���.
		printf("�� ������ �� �̻� �� �� �����ϴ�.\r\n", room_name.c_str() );
		
		sess->pre_send(room_full_msg, sizeof(room_full_msg));
	}
	else // �� ���� ���ɽ�...
	{
		room->set_name(std::move(room_name));

		chat_user* user = (chat_user*)(sess->get_user());

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// �ȳ� �޽��� ���
		sess->pre_send(enter_room_msg, sizeof(enter_room_msg));
	}
	
	// ������� ��� space(1) + id_size + '\r\n';
	return 1 + room_name_size + 2;
}

size_t telnet_parser::enter_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// space �ѱ� + 1byte
	char* room_name_str = &in_buffer[1];
	size -= 1;

	char* end_line_str = telnet_parser::find_enter_from_payload(room_name_str, size);
	if (nullptr == end_line_str) // ��ɾ� �ϼ� X 
	{
		return 0;
	}

	// room name ����.
	size_t	room_name_size = end_line_str - room_name_str;
	string	room_name{ room_name_str, room_name_size };


	// ���̸��� ���� ã��.
	chat_room* room = g_room_manager->find_room_using_name(room_name);
	if (nullptr == room) // ã�� ���� ���� ���.
	{
		sess->pre_send(not_find_room_msg, sizeof(not_find_room_msg)); // �ȳ� �޽��� ���.
	}
	else // �� ���� ���ɽ�...
	{
		chat_user* user = (chat_user*)sess->get_user();

		crash_if_false(nullptr != user);

		user->enter_room(room_name);

		// �ȳ� �޽��� ���
		sess->pre_send(enter_room_msg, sizeof(enter_room_msg));
	}

	// �� ������ ��� space(1) + room_name_size + '\r\n';
	return 1 + room_name_size + 2;
}

size_t telnet_parser::leave_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // ��ɾ� �ϼ� X 
	{
		return 0;
	}

	// user�� ���ϰ�...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	
	if (e_user_state::US_IN_ROOM != user->get_state()) // ���� �濡 ���� ���...
	{
		sess->pre_send(not_in_room_msg, sizeof(not_in_room_msg)); // �ȳ� �޽��� ���.
	}
	else // ���� �濡 �ִ� ���...
	{
		sess->pre_send(leave_user_from_room_msg, sizeof(leave_user_from_room_msg)); // �ȳ� �޽��� ���.
		user->leave_room();
	}
	
	// �� �������� ��� '\r\n'(2byte);
	return 2;
}



size_t telnet_parser::select_rooms_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // ��ɾ� �ϼ� X 
	{
		return 0;
	}

	// user�� ���ϰ�...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// �� ����Ʈ...
	string&& active_room_list = g_room_manager->get_active_room_list_to_string();
	if (active_room_list == "")
	{
		active_room_list = "���� ������ ���� �����ϴ�.\r\n";
	}


	sess->pre_send(active_room_list.c_str(), active_room_list.size()); // �ȳ� �޽��� ���.

	// �� �������� ��� '\r\n'(2byte);
	return 2;
}

size_t telnet_parser::select_user_in_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // ��ɾ� �ϼ� X 
	{
		return 0;
	}


	// user�� ���ϰ�...
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);
	chat_room* room = (chat_room*)user->get_room();
	crash_if_false(nullptr != room);
	

	if (e_user_state::US_IN_ROOM != user->get_state()) // ���� �濡 ���� ���...
	{
		sess->pre_send(not_find_room_msg, sizeof(not_find_room_msg)); // �ȳ� �޽��� ���.
	}
	else // ���� �濡 �ִ� ���...
	{
		// �� ����Ʈ...
		string user_list_in_room = room->get_user_list_in_room_to_string();
		if (user_list_in_room == "")
		{
			user_list_in_room = "���� �濡 ������ �����ϴ�.\r\n";
		}

		sess->pre_send(user_list_in_room.c_str(), user_list_in_room.size()); // �ȳ� �޽��� ���.
	}


	// ��� ���� ��� ��ȸ�� ��� '\r\n'(2byte);
	return 2;
}

size_t telnet_parser::select_all_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // ��ɾ� �ϼ� X 
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
		// ������ �α��� ���� ����...
		all_user_list = "���� ������ �����ϴ�.\r\n";
	}

	sess->pre_send(all_user_list.c_str(), all_user_list.size()); // �ȳ� �޽��� ���.


	return 2;
}



size_t telnet_parser::whisper_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	// user�� ���ϰ�
	chat_user* user = (chat_user*)sess->get_user();
	crash_if_false(nullptr != user);

	// space �ѱ� + 1byte
	char* target_name_str = &in_buffer[1];
	size -= 1;



	//��ɾ� ��
	char* end_line_str = telnet_parser::find_enter_from_payload(target_name_str, size);
	if (nullptr == end_line_str) // �̻��� ��ɾ�
	{
		crash();
		printf("��ȿ X ��ɾ� \r\n");
		return 0;
	}


	// ���� space
	char* next_space_str = telnet_parser::find_space_from_payload(target_name_str, size);
	if (nullptr == next_space_str) // ��ȿ���� ���� ��ɾ�...  
	{
		sess->pre_send(invalid_cmd_msg, sizeof(invalid_cmd_msg));

		return (end_line_str + 2) - in_buffer;
	}


	// string target name �����.
	size_t target_name_size = next_space_str - target_name_str;
	string target_name{target_name_str, target_name_size};
	
	chat_server* server = (chat_server*)sess->get_server();
	crash_if_false(nullptr != server);

	chat_user* target_user = server->find_user_using_name(target_name);
	if (nullptr == target_user) // ã�� ������ ���ٸ�?
	{
		// ã�� ��� ������ ���ٴ°��� �˸�.
		sess->pre_send(not_find_user_msg, sizeof(not_find_user_msg));

		return 0;
	}

	chat_session* target_session = (chat_session*)(target_user->get_session());
	crash_if_false(nullptr != target_session);

	//target_session->parse_send();
	// space ���� ��ġ
	next_space_str += 1;
	size -= (target_name_size + 1);


	// space �ѱ� + 1byte
	char* whisper_msg_str = next_space_str;
	size_t whisper_msg_size = end_line_str - next_space_str;
	string whisper_msg{next_space_str, whisper_msg_size};

	whisper_msg = user->get_name() + "���� �ӼӸ� : " + whisper_msg;
	whisper_msg += "\r\n";
	whisper_msg += end_msg;

	// ���� ��û.
	target_session->pre_send(whisper_msg.c_str(), whisper_msg.size());

	return 2;
}

size_t telnet_parser::chatting_handler(chat_session* sess, char* in_buffer, size_t size)
{
	crash_if_false(((size_t)(sess)+(size_t)in_buffer) != (size_t)in_buffer);

	//��ɾ� ��
	char* end_line_str = telnet_parser::find_enter_from_payload(in_buffer, size);
	if (nullptr == end_line_str) // ��ɾ� �ϼ� X 
	{
		crash();
		printf("��ȿ X ��ɾ� \r\n");
		return 0;
	}

	char*  msg_str = in_buffer;
	if (' ' != *msg_str && '\r' != *msg_str)
	{
		return 0;
	}

	msg_str = &in_buffer[1];
	size_t msg_size = end_line_str - msg_str;
	string msg{ msg_str, msg_size };


	// �α��� �� user�� ����.

	// user lobby ó��.
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

	// �α����� ��� space(1) + id_size + '\r\n';
	return 1 + msg_size + 2;
}
