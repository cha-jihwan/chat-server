#pragma once

enum e_packet_type
{
	PT_LOGIN,
	PT_LOGOUT,
	PT_CREATE_ROOM,
	PT_DELETE_ROOM,
	PT_CHATTING,
	PT_LOOOKUP_ROOM,
	PT_LOOOKUP_LOBBY,
	PT_LOOOKUP_ALL,

	PT_MAX
};

struct null_packet {};
struct packet : public null_packet 
{
	e_packet_type type;
};