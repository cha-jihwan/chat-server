#pragma once


enum e_packet_type
{
	PT_MAX,
};

struct message_view
{
	const char* msg;
	size_t		size;
};