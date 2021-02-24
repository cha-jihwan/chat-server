#pragma once



namespace c2 { namespace server { namespace contents
{
	char gui_header[118] =
		"------------------------------------------\n\n\n\n 채팅서버 \n\n 넷마블네오 \n\n ------------------------------------------\r\n";
	char gui_body[191] =
		"/L [ID] 로그인(미구현)\r\n/CR [ROOM_NAME]  방생성(미구현) \r\n/SR 방 목록(미구현) \r\n/C 채팅(echo로만 구현) \r\n/W [TARGET ID]귓속말 \r\n/SRU 방 참여자 목록(미구현) \r\n/SUA 접속자 목록(미구현)\r\n";



	//const char* login{ "/LOGIN" };
	//const char* a{ "/login" };


} // namespace contents
} // namespace server
} // namespace 
