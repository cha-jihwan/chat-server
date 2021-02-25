#pragma once



namespace c2 { namespace server { namespace contents
{
	extern char gui_header[205];
	extern char gui_body[198];
	extern char end_msg[2];

	// for enter room
	extern char room_full_msg[62];
	extern char enter_room_msg[32];

	extern char not_find_room_msg[27];
	extern char not_in_room_msg[41];
	extern char leave_user_from_room_msg[35];

	extern char leave_room_msg[21];

	extern char user_in_room_header_msg[14];
	extern char invalid_cmd_msg[31];
	extern char not_find_user_msg[31];

	extern char already_closed_msg[24];
	extern char leave_room_msg2[22];

	extern char before_login_msg[48];
	/*const char* login{ "/LOGIN" };
	const char* a{ "/login" };
*/

} // namespace contents
} // namespace server
} // namespace 
