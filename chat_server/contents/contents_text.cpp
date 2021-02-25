#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents
{
	char gui_header[205] =
	"------------------------------------------------------------------------------------\r\n\n\n\n 채팅서버 \n\n 넷마블네오 \n\n------------------------------------------------------------------------------------\r\n\r\n";
	char gui_body[201] =
		"/L [ID]			로그인\r\n/R [ROOM_NAME]		방생성 \r\n/q [ROOM_NAME]		방접속\r\n/e			방 나가기 \r\n/S			방 목록 조회 \r\n/C	[msg]		채팅 \r\n/W [TARGET ID] [msg]	귓속말\r\n/U			방 참여자 목록 \r\n/A			접속자 목록\r\n";
	char end_msg[2]	{ 0, 0};
	char room_full_msg[62] = "방이 전부 생성되서 빈방이 없습니다. 기존 방을 이용해주세요.\r\n";
	char enter_room_msg[32] = "방 생성 후 방에 접속했습니다.\r\n";
	char overlapped_login_msg[32] = "중복 로그인 시도 중이십니다..\r\n";


	char not_find_room_msg[27] = "찾으시는 방이 없습니다..\r\n";
	char not_in_room_msg[41] = "현재 방에 접속해 있는 상태가 아닙니다.\r\n";
	char laeve_room_msg[21] = "방에서 나갔습니다.\r\n";
	char leave_user_from_room_msg[35] = "방에서 나갔습니다. 로비로 갑니다\r\n";

	char user_in_room_header_msg[14] = "참여자 명단\r\n";
	char invalid_cmd_msg[31] = "유효하지 않은 명령어 입니다.\r\n";
	char not_find_user_msg[31] = "찾으시는 상대방이 없습니다..\r\n";
	
	char before_login_msg[48] = "아직 로그인 되지 않았습니다 로그인 해주세요..\r\n";
	char already_closed_msg[24] = "이미 종료 되었습니다.\r\n";
	char laeve_room_msg2[22] = "1방에서 나갔습니다.\r\n";

	char clear_console[73] = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";


	char kick_message[58] = "더 이상 들어올 수 없습니다. 다음에 다시 이용해주세요.\r\n\0\0";
} // namespace contents
} // namespace server
} // namespace 
