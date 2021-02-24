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
		size_t current_cmd_size{};
		
		// ���� ����� �ּ� ��� ������ ���� �۴ٸ� ����.
		// 4 = commnand_size (2) + \r\n (2)
		if (4 > in_size - parsed_size)
		{
			return parsed_size;
		}

		string cmd{&in_buffer[parsed_size], 2};
		
		// 1���� ���� ���...
		if (1 > packet_handlers.count(cmd))
		{
			current_cmd_size += telnet_parser::invalid_packet_handler(in_session, &in_buffer[parsed_size + 2], in_size);
		}
		else
		{
			current_cmd_size += packet_handlers[cmd](in_session, &in_buffer[parsed_size + 2], in_size);
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

	//// ä��
	packet_handlers.emplace("/C", telnet_parser::chatting_handler);
	packet_handlers.emplace("/c", telnet_parser::chatting_handler);

	//// �� ���
	packet_handlers.emplace("/S", telnet_parser::select_room_handler);
	packet_handlers.emplace("/s", telnet_parser::select_room_handler);

	//// �ӼӸ�
	packet_handlers.emplace("/W", telnet_parser::whisper_handler);
	packet_handlers.emplace("/w", telnet_parser::whisper_handler);

	//// �� ������ ���
	packet_handlers.emplace("/S", telnet_parser::select_user_in_room_handler);
	packet_handlers.emplace("/s", telnet_parser::select_user_in_room_handler);

	//// ������ ���
	packet_handlers.emplace("/U", telnet_parser::select_all_handler);
	packet_handlers.emplace("/u", telnet_parser::select_all_handler);
}

//
//// payload�� ���� ���ڸ� ã�´�.
//// return value 
//// ���ڿ��� ã�´ٸ� ��������
//// ��ã�´ٸ� nullptr
//// ���ڸ� ã�� ���Ѵٸ� nullptr
//// ���ڸ� ã�´ٸ� ��ġ ���� ����.
//char* telnet_parser::find_space_from_payload(char* in_buf, size_t in_size)
//{
//	for (size_t n{}; n < in_size; ++n, ++in_buf)
//	{
//		if (in_buf[n] == ' ')
//		{
//			return &in_buf[n];
//		}
//	}
//
//	return nullptr;
//}
//
//char* telnet_parser::find_enter_from_payload(char* in_buf, size_t in_size)
//{
//	return nullptr;
//}
//


//// payload�� ���� telnet�� enter�Է� (linfeed + carage-return)�� ã�´�.
//// \r\n 
//// return value 
//// ���ڿ��� ã�´ٸ� ��������
//// ��ã�´ٸ� nullptr
char* telnet_parser::find_enter_from_payload(char* in_payload, size_t in_size)
{
	uint16_t* in_buf = reinterpret_cast<uint16_t*>(in_payload);
	size_t founded_size = in_size;

	for (size_t n{}; (n * 2) < in_size; ++n)
	{
		if (in_payload[n] != '\r\n')
		{
			return &in_payload[n];
		}
	}

	return nullptr;
}
//



size_t telnet_parser::login_handler(chat_session* sess, char* in_buffer, size_t size)
{
	in_buffer += 1;

	

	chat_user* user = reinterpret_cast<chat_user*>(sess->get_user());

	user->enter_lobby();

	return size_t();
}

size_t telnet_parser::create_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::enter_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::leave_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::whisper_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::chatting_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::select_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::select_user_in_room_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::select_all_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}

size_t telnet_parser::invalid_packet_handler(chat_session* sess, char* in_buffer, size_t size)
{
	return size_t();
}
