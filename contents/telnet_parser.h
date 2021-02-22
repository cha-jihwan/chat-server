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
		// 최소 명령어 크기 case 1: 1 + N + 2
		// case 1:'\'(1) + cmd(가변 N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 2:'\'(1) + cmd(가변 N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 3:'\'(1) + cmd(가변 N) + 1(sapce) + 상대ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 1: command \r\n
		// case 2: command space body \r\n
		// case 3: command space target_name body \r\n


		// 메시지가 유효한가? \d\a 찾고
		// find_str(buffer, "\d\a");

		// space 찾고
		// find_ch(buffer, ' ');

		// cmd 분리
		// cmd가 유효한가?

		// 메시지 타입 넣고
		// switch(msg_type)
		// case1인 경우 헤더만 넣어서 msg제작 끝
		// case2인 경우 헤더 바디 분리 
		// case3인 경우 헤더 target_id 바디 분리.

	}
	return parsed_size;
}
