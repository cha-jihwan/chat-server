#pragma once

class telnet_parser : public i_parser<packet>
{
public:
	static size_t parse_payload(char* in_buffer, size_t in_size, std::vector<packet>& out_buffer);
private:
	static char* find_character_from_payload(char*& in_buf, size_t in_size, const char ch);
	static char* find_string_from_payload(char*& in_buf, size_t in_size, const char* str);
};

inline size_t telnet_parser::parse_payload(char* in_buffer, size_t in_size, std::vector<packet>& out_buffer)
{
	size_t	parsed_size{};
	char*	buf_ptr = in_buffer;
	packet  pkt;

	for (;;)
	{
		// �ּ� ��ɾ� ũ�� case 1: 1 + N + 2
		// case 1:'\'(1) + cmd(���� N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 2:'\'(1) + cmd(���� N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 3:'\'(1) + cmd(���� N) + 1(sapce) + ���ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 1: command \r\n
		// case 2: command space body \r\n
		// case 3: command space target_name body \r\n
		char* cmd = find_character_from_payload( buf_ptr, in_size - parsed_size, ' ');
		if ( nullptr == cmd ) // cmd ������ return;
		{
			return parsed_size;
		}

		// type parsing
		pkt.type;


		// cmd ����
		char* body  = find_string_from_payload(cmd, in_size - (in_buffer - cmd), "\r\n");
		if (nullptr == cmd) // cmd ������ return;
		{
				
		}

		pkt.msg;


		// �޽����� ��ȿ�Ѱ�? \d\a ã��
		// find_str(buffer, "\d\a");

		// space ã��
		// find_ch(buffer, ' ');

		// cmd �и�
		// cmd�� ��ȿ�Ѱ�?

		// �޽��� Ÿ�� �ְ�
		// switch(msg_type)
		// case1�� ��� ����� �־ msg���� ��
		// case2�� ��� ��� �ٵ� �и� 
		// case3�� ��� ��� target_id �ٵ� �и�.

	}

	return parsed_size;
}

// return value 
// ���ڿ��� ã�� ���Ѵٸ� nullptr
// ���ڿ��� ã�´ٸ� ��ġ ���� ����.

// ���ڿ� ã�� ���� �Լ�.

inline char* telnet_parser::find_character_from_payload(char*& in_buf, size_t in_size, const char ch)
{
	for (int n{}; n < in_size; ++n)
	{
		if (in_buf[n] == ch)
		{
			return &in_buf[n];
		}
	}

	return nullptr;
}

inline char* telnet_parser::find_string_from_payload(char*& in_payload, size_t in_size, const char* str)
{
	for (int n{}; n < in_size; ++n)
	{
	}



	return nullptr;
}

