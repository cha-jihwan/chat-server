#pragma once
#include <string>
// constens에 필요한 문자열 모음.
namespace c2 { namespace server { namespace contents
{
	// server gui
	extern std::string gui_header;
	extern std::string gui_body;
	extern std::string end_msg;
	
	//
	extern std::string already_closed_msg;//[24];
	extern std::string before_login_msg;//[47];
	extern std::string blank_id_msg;//[75];
	extern std::string create_room_msg;//[26];
	extern std::string enter_room_msg;//[21];
	extern std::string overlapped_login_msg;//[31];
	
	extern std::string invalid_cmd_msg;//[31];
	extern std::string kick_message;//[56];
	extern std::string login_msg;//[29];
	extern std::string leave_user_from_room_msg;//[35];
	extern std::string leave_room_msg;//[21];
	extern std::string leave_room_msg2;//[22];

	extern std::string request_login_msg;//[51];
	extern std::string room_full_msg;
	extern std::string request_id_msg;

	extern std::string not_find_user_msg;//[31];
	extern std::string not_find_room_msg;//[27];
	extern std::string not_in_room_msg;//[41];
	
	extern std::string user_in_room_header_msg;//[20];



	extern const std::string packet_filter_keywords[9];

	enum class e_packet_filter
	{
		EPF_LOGIN_OK,
		EPF_ROOM_CREATING_OK,
		EPF_ROOM_EXITING_OK,
		EPF_ROOM_ENTERING_OK,
		EPF_CHAT,
		EPF_WHISPER,
		EPF_SELECT_ROOM_LIST,
		EPF_SELECT_USER_LIST,
		EPF_SELECT_USER_LIST_IN_ROOM,
		EPF_MAX,
	};
} // namespace contents
} // namespace server
} // namespace 
