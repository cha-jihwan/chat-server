#pragma once

// constens에 필요한 문자열 모음.
namespace c2 { namespace server { namespace contents
{
	// server gui
	extern char gui_header[206];
	extern char gui_body[202];
	extern char end_msg[2];
	
	//
	extern char already_closed_msg[24];
	extern char before_login_msg[48];
	extern char blank_id_msg[75];
	extern char create_room_msg[28];
	extern char enter_room_msg[21];
	extern char overlapped_login_msg[32];
	
	extern char invalid_cmd_msg[31];
	extern char kick_message[56];
	extern char login_msg[29];
	extern char leave_user_from_room_msg[35];
	extern char leave_room_msg[21];
	extern char leave_room_msg2[22];

	extern char request_login_msg[51];
	extern char room_full_msg[62];
	extern char request_id_msg[25];

	extern char not_find_user_msg[31];
	extern char not_find_room_msg[27];
	extern char not_in_room_msg[41];
	
	extern char user_in_room_header_msg[14];

} // namespace contents
} // namespace server
} // namespace 
