#pragma once

using namespace c2::server::contents;

class telnet_parser : public i_parser<packet>
{
	using packet_handler = size_t(*)(chat_session*, char*, size_t);
	using ttt = size_t(*)();

public:
	static size_t parse_payload(chat_session* in_session, char* in_buffer, size_t in_size, std::vector<packet>& out_buffer);
	static void initialize();

private:
	//static char* find_space_from_payload(char* in_buf, size_t in_size);
	static char* find_enter_from_payload(char* in_buf, size_t in_size);
	//static char* find_body(char* in_buf, size_t in_size, packet& out_packet);
	//static char* find_option(char* in_buf, size_t in_size, packet& out_packet);

	static unordered_map<std::string, packet_handler> packet_handlers;

private:
	static size_t login_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t create_room_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t enter_room_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t leave_room_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t whisper_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t chatting_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t select_room_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t select_user_in_room_handler(chat_session* sess, char* in_buffer, size_t size);
	static size_t select_all_handler(chat_session* sess, char* in_buffer, size_t size);

	static size_t invalid_packet_handler(chat_session* sess, char* in_buffer, size_t size);
};

