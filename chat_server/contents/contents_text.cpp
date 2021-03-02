#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents
{
	std::string gui_header =
	"------------------------------------------------------------------------------------\r\n\n\n\n 채팅서버 \n\n 넷마블네오 \r\n\r\n------------------------------------------------------------------------------------\r\n\r\n";
	std::string gui_body =
		"/L [ID]			로그인\r\n/R [ROOM_NAME]		방생성 \r\n/Q [ROOM_NAME]		방 접속\r\n/E			방 나가기 \r\n/S			방 목록 조회 \r\n/C [msg]		채팅 \r\n/W [TARGET ID] [msg]	귓속말\r\n/U			방 참여자 목록 \r\n/A			접속자 목록\r\n";
	std::string end_msg	{ '\r', '\n'};
	std::string room_full_msg = "방이 전부 생성되서 빈방이 없습니다. 기존 방을 이용해주세요.\r\n";
	std::string enter_room_msg  = "방에 접속했습니다.\r\n";
	std::string overlapped_login_msg = "중복 로그인 시도 중이십니다.\r\n";

	std::string not_find_room_msg = "찾으시는 방이 없습니다..\r\n";
	std::string not_in_room_msg = "현재 방에 접속해 있는 상태가 아닙니다.\r\n";
	std::string laeve_room_msg = "방에서 나갔습니다.\r\n";
	std::string leave_user_from_room_msg = "방에서 나갔습니다. 로비로 갑니다\r\n";

	std::string user_in_room_header_msg = "방 참여 유저 목록\r\n";
	std::string invalid_cmd_msg = "유효하지 않은 명령어 입니다.\r\n";
	std::string blank_id_msg = "로그인 실패 아이디는 중간에 공백이 포함되면 안됩니다. 다시 시도해주세요.\r\n";
	std::string request_id_msg = "아이디를 입력해주세요.\r\n";
	std::string request_login_msg = "로그인하지 않은 상태입니다. 로그인부터 해주세요.\r\n";
	std::string not_find_user_msg = "찾으시는 상대방이 없습니다..\r\n";
	
	std::string before_login_msg   = "아직 로그인 되지 않았습니다 로그인 해주세요.\r\n";
	std::string already_closed_msg = "이미 종료 되었습니다.\r\n";
	std::string laeve_room_msg2 = "방 탈출 하였습니다.\r\n";
	std::string login_msg = "님 로그인 성공 하셨습니다.\r\n";
	std::string create_room_msg = "방 생성 성공하였습니다.\r\n";
	std::string clear_console = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";


	std::string kick_message = "더 이상 들어올 수 없습니다. 다음에 다시 이용해주세요.\r\n";



	const std::string packet_filter_keywords[(size_t)e_packet_filter::EPF_MAX]
	{
		"로그인 성공\r\n",
		"방 생성 성공\r\n",
		"방 탈출 성공\r\n",
		"방 참가 성공\r\n",
		"님의 채팅 : ",
		"님의 귓속말 : ",
		"전체 방 목록\r\n",
		"전체 유저 목록\r\n",
		"방 참여 유저 목록\r\n",
	};

} // namespace contents
} // namespace server
} // namespace 
