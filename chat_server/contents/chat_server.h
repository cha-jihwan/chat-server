#pragma once

namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core;

	// ä�� ����.
	class chat_server : public select_server 
	{
	public:
		// ������
		chat_server();
		// �Ҹ���
		virtual ~chat_server() override final;

		// ä�� ������ �ʿ��� �ʱ�ȭ�� ����ϴ� �Լ�.
		virtual bool initialize_contents();
		
		// user�� sesion�� ���� �Ҵ� �Լ�.
		virtual session* allocate_session() override final;
		virtual i_user* allocate_user(const string& user_name) override final;

		// user�� session�� ���� �Ҵ� ���� �Լ�.
		virtual void free_session(session* sess) override final; 
		virtual void free_user(i_user* user)     override final;

		// accpet ó��
		virtual bool on_accept(SOCKET sock)	override final;

		// session�� accept()�� ������ �������ܿ��� �ʿ��� ó��
		virtual void on_join(session* sess)	override final;
		// session�� ������ ���� ������ �������ܿ��� �ʿ��� ó��
		virtual void on_disconnect(session* sess)	override final;

		// user �� active_table�� �߰�.
		// ������ �Ǿ��ٸ� true�� ����.
		bool register_user(const string& user_name, chat_user* user);

		// user �� active_table�� �߰�.
		// ������ �Ǿ��ٸ� true�� ����.
		void unregister_user(chat_user* user);

		// �̸��� ���� user�� ã�´�.
		chat_user*	find_user_using_name(const string& name);

		// ���� Ȱ��ȭ�� �������� ����� 
		// "user_name\r\n" ������ string��ü�� ���� ��ȯ
		string		get_active_user_to_string();

		// kick thread������ ó��.
		static void do_kick(HANDLE evnt, concurrency::concurrent_queue<SOCKET>* to_close_sockes);
		// server�� ���� ȣ���ϴ� ű ��û..
		void request_kick_socket(SOCKET sock);


	private:
		// ���� �Ҵ�� user�� name�� Ű�� �ϴ� user��ü ���� ���̺�.
		unordered_map<string, chat_user*>							active_user_table;

		// session�� user�� ������Ʈ Ǯ.
		bounded_object_pool<chat_session, constant::c_maximum_ccu>*	session_pool;
		bounded_object_pool<chat_user, constant::c_maximum_ccu> *	user_pool;


		// ű ó���� ����...
		concurrency::concurrent_queue<SOCKET>						to_kick_sockes;
		HANDLE														kick_event;
		thread*														kick_thread;
	};
} // namespace contents
} // namespace server
} // namespace c2
