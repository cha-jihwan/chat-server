#pragma once



namespace c2 { namespace server { namespace contents
{
	char gui_header[118] =
		"------------------------------------------\n\n\n\n ä�ü��� \n\n �ݸ���׿� \n\n ------------------------------------------\r\n";
	char gui_body[191] =
		"/L [ID] �α���(�̱���)\r\n/CR [ROOM_NAME]  �����(�̱���) \r\n/SR �� ���(�̱���) \r\n/C ä��(echo�θ� ����) \r\n/W [TARGET ID]�ӼӸ� \r\n/SRU �� ������ ���(�̱���) \r\n/SUA ������ ���(�̱���)\r\n";

	char room_full_msg[62] = "���� ���� �����Ǽ� ����� �����ϴ�. ���� ���� �̿����ּ���.\r\n";
	char enter_room_msg[32] = "�� ���� �� �濡 �����߽��ϴ�.\r\n";
	
	char not_find_room_msg[27] = "ã���ô� ���� �����ϴ�..\r\n";
	char not_in_room_msg[41] = "���� �濡 ������ �ִ� ���°� �ƴմϴ�.\r\n";
	char laeve_room_msg[21] = "�濡�� �������ϴ�.\r\n";
	char leave_user_from_room_msg[35] = "�濡�� �������ϴ�. �κ�� ���ϴ�\r\n";


	char user_in_room_header_msg[14] = "������ ���\r\n";
	char invalid_cmd_msg[31] = "��ȿ���� ���� ��ɾ� �Դϴ�.\r\n";
	char not_find_user_msg[31] = "ã���ô� ������ �����ϴ�..\r\n";
	
	char already_closed_msg[24] = "�̹� ���� �Ǿ����ϴ�.\r\n";
	char laeve_room_msg2[22] = "1�濡�� �������ϴ�.\r\n";

	//const char* login{ "/LOGIN" };
	//const char* a{ "/login" };


} // namespace contents
} // namespace server
} // namespace 
