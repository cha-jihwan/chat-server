#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents
{
	std::string gui_header =
	"------------------------------------------------------------------------------------\r\n\n\n\n ä�ü��� \n\n �ݸ���׿� \r\n\r\n------------------------------------------------------------------------------------\r\n\r\n";
	std::string gui_body =
		"/L [ID]			�α���\r\n/R [ROOM_NAME]		����� \r\n/Q [ROOM_NAME]		�� ����\r\n/E			�� ������ \r\n/S			�� ��� ��ȸ \r\n/C [msg]		ä�� \r\n/W [TARGET ID] [msg]	�ӼӸ�\r\n/U			�� ������ ��� \r\n/A			������ ���\r\n";
	std::string end_msg	{ '\r', '\n'};
	std::string room_full_msg = "���� ���� �����Ǽ� ����� �����ϴ�. ���� ���� �̿����ּ���.\r\n";
	std::string enter_room_msg  = "�濡 �����߽��ϴ�.\r\n";
	std::string overlapped_login_msg = "�ߺ� �α��� �õ� ���̽ʴϴ�.\r\n";

	std::string not_find_room_msg = "ã���ô� ���� �����ϴ�..\r\n";
	std::string not_in_room_msg = "���� �濡 ������ �ִ� ���°� �ƴմϴ�.\r\n";
	std::string laeve_room_msg = "�濡�� �������ϴ�.\r\n";
	std::string leave_user_from_room_msg = "�濡�� �������ϴ�. �κ�� ���ϴ�\r\n";

	std::string user_in_room_header_msg = "�� ���� ���� ���\r\n";
	std::string invalid_cmd_msg = "��ȿ���� ���� ��ɾ� �Դϴ�.\r\n";
	std::string blank_id_msg = "�α��� ���� ���̵�� �߰��� ������ ���ԵǸ� �ȵ˴ϴ�. �ٽ� �õ����ּ���.\r\n";
	std::string request_id_msg = "���̵� �Է����ּ���.\r\n";
	std::string request_login_msg = "�α������� ���� �����Դϴ�. �α��κ��� ���ּ���.\r\n";
	std::string not_find_user_msg = "ã���ô� ������ �����ϴ�..\r\n";
	
	std::string before_login_msg   = "���� �α��� ���� �ʾҽ��ϴ� �α��� ���ּ���.\r\n";
	std::string already_closed_msg = "�̹� ���� �Ǿ����ϴ�.\r\n";
	std::string laeve_room_msg2 = "�� Ż�� �Ͽ����ϴ�.\r\n";
	std::string login_msg = "�� �α��� ���� �ϼ̽��ϴ�.\r\n";
	std::string create_room_msg = "�� ���� �����Ͽ����ϴ�.\r\n";
	std::string clear_console = "\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n";


	std::string kick_message = "�� �̻� ���� �� �����ϴ�. ������ �ٽ� �̿����ּ���.\r\n";



	const std::string packet_filter_keywords[(size_t)e_packet_filter::EPF_MAX]
	{
		"�α��� ����\r\n",
		"�� ���� ����\r\n",
		"�� Ż�� ����\r\n",
		"�� ���� ����",
		"���� ä�� : ",
		"���� �ӼӸ� : ",
		"��ü �� ���\r\n",
		"��ü ���� ���\r\n",
		"�� ���� ���� ���",
	};

} // namespace contents
} // namespace server
} // namespace 
