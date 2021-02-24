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
	size_t	parsed_size{};
	char*	buf_ptr = in_buffer;
	packet  pkt;

	for (;;)
	{
		size_t current_cmd_size{};
		// 최소 명령어 크기 case 1: 1 + N + 2
		// case 1:'\'(1) + cmd(가변 N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 2:'\'(1) + cmd(가변 N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 3:'\'(1) + cmd(가변 N) + 1(sapce) + 상대ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
		// case 1: command \r\n
		// case 2: command space body \r\n
		// case 3: command space target_name body \r\n
		char* cmd = find_space_from_payload( buf_ptr, in_size - parsed_size);
		if (nullptr == cmd) // cmd 없으면 return;
		{
			return parsed_size;
		}

		// 유효한 명령어 아니면?
		if (false == cmd) // 함수 구현 필요...
		{

		}


		// cmdㅈ
		// type parsing
		pkt.type;


		current_cmd_size += (buf_ptr - cmd); // += cmd size

		

		// cmd 저장
		char* body = find_enter_from_payload(cmd+1, in_size - current_cmd_size);
		if (nullptr == body) // body 없으면 return;
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

		parsed_size += current_cmd_size;
	}

	return parsed_size;
}




// payload로 부터 문자를 찾는다.
// return value 
// 문자열을 찾는다면 시작지점
// 못찾는다면 nullptr
// 문자를 찾지 못한다면 nullptr
// 문자를 찾는다면 위치 시작 지점.
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



// payload로 부터 telnet의 enter입력 (linfeed + carage-return)를 찾는다.

// \r\n 
// return value 
// 문자열을 찾는다면 시작지점
// 못찾는다면 nullptr
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

