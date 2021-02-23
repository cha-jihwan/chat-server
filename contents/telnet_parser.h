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
		// 최소 명령어 크기 case 1: 1 + N + 2
		// case 1:'\'(1) + cmd(가변 N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 2:'\'(1) + cmd(가변 N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 3:'\'(1) + cmd(가변 N) + 1(sapce) + 상대ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 1: command \r\n
		// case 2: command space body \r\n
		// case 3: command space target_name body \r\n
		char* cmd = find_character_from_payload( buf_ptr, in_size - parsed_size, ' ');
		if ( nullptr == cmd ) // cmd 없으면 return;
		{
			return parsed_size;
		}

		// type parsing
		pkt.type;


		// cmd 저장
		char* body  = find_string_from_payload(cmd, in_size - (in_buffer - cmd), "\r\n");
		if (nullptr == cmd) // cmd 없으면 return;
		{
				
		}

		pkt.msg;


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

// return value 
// 문자열을 찾지 못한다면 nullptr
// 문자열을 찾는다면 위치 시작 지점.

// 문자열 찾는 안전 함수.

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

