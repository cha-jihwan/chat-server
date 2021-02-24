#pragma once

class telnet_parser : public i_parser<packet>
{
public:
	static size_t parse_payload(char* in_buffer, size_t in_size, std::vector<packet>& out_buffer);
private:
	static char* find_space_from_payload(char* in_buf, size_t in_size);
	static char* find_enter_from_payload(char* in_buf, size_t in_size);

	static char* find_body(char* in_buf, size_t in_size, packet& out_packet);
	static char* find_option(char* in_buf, size_t in_size, packet& out_packet);
};

inline size_t telnet_parser::parse_payload(char* in_buffer, size_t in_size, std::vector<packet>& out_buffer)
{
	//packet  pkt2;
	//map<string_view, int> m;
	//m[pkt2.msg];

	size_t	parsed_size{};
	char*	buf_ptr = in_buffer;
	packet  pkt;

	for (;;)
	{
		size_t current_cmd_size{};
		// �ּ� ��ɾ� ũ�� case 1: 1 + N + 2
		// case 1:'\'(1) + cmd(���� N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 2:'\'(1) + cmd(���� N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 3:'\'(1) + cmd(���� N) + 1(sapce) + ���ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 1: command \r\n
		// case 2: command space body \r\n
		// case 3: command space target_name body \r\n
		char* cmd = find_space_from_payload( buf_ptr, in_size - parsed_size);
		if (nullptr == cmd) // cmd ������ return;
		{
			return parsed_size;
		}

		// ��ȿ�� ��ɾ� �ƴϸ�?
		if (false == cmd) // �Լ� ���� �ʿ�...
		{

		}


		// cmd��
		// type parsing
		pkt.type;


		current_cmd_size += (buf_ptr - cmd); // += cmd size

		

		// cmd ����
		char* body = find_enter_from_payload(cmd+1, in_size - current_cmd_size);
		if (nullptr == body) // body ������ return;
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

		parsed_size += current_cmd_size;
	}

	return parsed_size;
}




// payload�� ���� ���ڸ� ã�´�.
// return value 
// ���ڿ��� ã�´ٸ� ��������
// ��ã�´ٸ� nullptr
// ���ڸ� ã�� ���Ѵٸ� nullptr
// ���ڸ� ã�´ٸ� ��ġ ���� ����.
inline char* telnet_parser::find_space_from_payload(char* in_buf, size_t in_size)
{
	for (int n{}; n < in_size; ++n, ++in_buf)
	{
		if (in_buf[n] == ' ')
		{
			return &in_buf[n];
		}
	}

	return nullptr;
}



// payload�� ���� telnet�� enter�Է� (linfeed + carage-return)�� ã�´�.

// \r\n 
// return value 
// ���ڿ��� ã�´ٸ� ��������
// ��ã�´ٸ� nullptr
inline char* telnet_parser::find_enter_from_payload(char* in_payload, size_t in_size)
{
	uint16_t* in_buf = reinterpret_cast<uint16_t*>(in_payload);
	size_t founded_size = in_size;

	for (int n{}; (n * 2) < in_size; ++n)
	{
		if (in_payload[n] != '\r\n')
		{
			return &in_payload[n];
		}
	}

	return nullptr;
}

