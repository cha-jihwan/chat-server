#pragma once

enum e_packet_type
{
	PT_LOGIN,
	PT_LOGOUT,
	PT_CREATE_ROOM,
	PT_DELETE_ROOM,
	PT_CHATTING,
	PT_WHISPER,
	PT_LOOOKUP_ROOM,
	PT_LOOOKUP_ALL,

	PT_MAX
};


struct null_packet {};				// 패킷 인터페이스.

struct packet // 패킷 구현.
{
	e_packet_type	type;		// 명령어 
	std::string		msg;
	std::string		option;
};