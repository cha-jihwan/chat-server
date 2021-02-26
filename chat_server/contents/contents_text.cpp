#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents
{
	char gui_header[206] =
	"------------------------------------------------------------------------------------\r\n\n\n\n ä�ü��� \n\n �ݸ���׿� \r\n\r\n------------------------------------------------------------------------------------\r\n\r\n";
	char gui_body[202] =
		"/L [ID]			�α���\r\n/R [ROOM_NAME]		����� \r\n/Q [ROOM_NAME]		�� ����\r\n/E			�� ������ \r\n/S			�� ��� ��ȸ \r\n/C [msg]		ä�� \r\n/W [TARGET ID] [msg]	�ӼӸ�\r\n/U			�� ������ ��� \r\n/A			������ ���\r\n";
	char end_msg[2]	{ 0, 0};
	char room_full_msg[62] = "���� ���� �����Ǽ� ����� �����ϴ�. ���� ���� �̿����ּ���.\r\n";
	char enter_room_msg[21] = "�濡 �����߽��ϴ�.\r\n";
	char overlapped_login_msg[32] = "�ߺ� �α��� �õ� ���̽ʴϴ�..\r\n";

	char not_find_room_msg[27] = "ã���ô� ���� �����ϴ�..\r\n";
	char not_in_room_msg[41] = "���� �濡 ������ �ִ� ���°� �ƴմϴ�.\r\n";
	char laeve_room_msg[21] = "�濡�� �������ϴ�.\r\n";
	char leave_user_from_room_msg[35] = "�濡�� �������ϴ�. �κ�� ���ϴ�\r\n";

	char user_in_room_header_msg[14] = "������ ���\r\n";
	char invalid_cmd_msg[31] = "��ȿ���� ���� ��ɾ� �Դϴ�.\r\n";
	char blank_id_msg[63] = "���̵�� �߰��� ������ ���ԵǸ� �ȵ˴ϴ�. �ٽ� �õ����ּ���.\r\n";
	char request_id_msg[25] = "���̵� �Է����ּ���.\r\n";
	char request_login_msg[51] = "�α������� ���� �����Դϴ�. �α��κ��� ���ּ���.\r\n";
	char not_find_user_msg[31] = "ã���ô� ������ �����ϴ�..\r\n";
	
	char before_login_msg[48] = "���� �α��� ���� �ʾҽ��ϴ� �α��� ���ּ���..\r\n";
	char already_closed_msg[24] = "�̹� ���� �Ǿ����ϴ�.\r\n";
	char laeve_room_msg2[22] = "1�濡�� �������ϴ�.\r\n";
	char login_msg[29] = "�� �α��� ���� �ϼ̽��ϴ�.\r\n";
	char create_room_msg[28] = "�� ������ �����Ͽ����ϴ�.\r\n";
	char clear_console[73] = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";


	char kick_message[58] = "�� �̻� ���� �� �����ϴ�. ������ �ٽ� �̿����ּ���.\r\n\0\0";
} // namespace contents
} // namespace server
} // namespace 
