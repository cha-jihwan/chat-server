#pragma once

class telnet_parser : public i_parser<packet>
{
public:
	static size_t parse_payload(char* in_buffer, size_t in_size, std::vector<packet>& out_buffer);
};

inline size_t telnet_parser::parse_payload(char* in_buffer, size_t in_size, std::vector<packet>& out_buffer)
{
	size_t	parsed_size{};
	char*	buf_ptr = in_buffer;

	for (;;)
	{
		// �ּ� ��ɾ� ũ�� case 1: 1 + N + 2
		// case 1:'\'(1) + cmd(���� N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 2:'\'(1) + cmd(���� N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 3:'\'(1) + cmd(���� N) + 1(sapce) + ���ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 1: command \r\n
		// case 2: command space body \r\n
		// case 3: command space target_name body \r\n


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
