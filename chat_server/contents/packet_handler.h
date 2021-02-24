#pragma once


using namespace c2::server::core;

size_t login_handler(session* sess, char* in_buffer);
size_t create_room_handler(session* sess, char* in_buffer);
size_t enter_room_handler(session* sess, char* in_buffer);
size_t leave_room_handler(session* sess, char* in_buffer);
size_t whisper_handler( session* sess, char* in_buffer);
size_t chatting_handler(session* sess, char* in_buffer);
size_t select_room_handler(session* sess, char* in_buffer);
size_t select_user_in_room_handler(session* sess, char* in_buffer);
size_t select_all_handler(session* sess, char* in_buffer);
