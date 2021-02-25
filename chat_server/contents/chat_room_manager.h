#pragma once

namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core::constant;

	// �� ������ Ŭ����.
	// ���(�Ҵ� + ����) ���� �����Ѵ�. 
	class chat_room_manager
	{
	public:
		// ������
		chat_room_manager();
		// �Ҹ���
		~chat_room_manager();

		// �� �Ҵ���.
		chat_room* allocate(const string& room_name);
		void free(chat_room* room);


		// �� �̸��� ���� ���� ã���ϴ�.
		// param name : �� �̸�
		// return value 
		// if	nullptr �̸��� �´� ���� ���� ���
		// else			ã�� ���� ����
		chat_room* find_room_using_name(const string& name);
	
		// �� ����� ���ڿ��� "room_name" + "\r\n" ��ȯ���ش�.
		string get_active_room_list_to_string();

	private:
		// �Ҵ�� ���� �����ϴ� �ڷᱸ��
		unordered_map<string, chat_room*> active_room_table;
		// ������Ʈ Ǯ
		bounded_object_pool<chat_room, c_maximum_ccu> room_pool;
	};

	extern chat_room_manager* g_room_manager;
} // namespace contents
} // namespace server
} // namespace c2