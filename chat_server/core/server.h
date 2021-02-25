#pragma once

namespace c2 { namespace server { namespace core 
{
	// server�� base class 
	// ��� �ް� �����Լ��� ���� �߰� ����� �����Ѵ�.
	class select_server
	{
	public:
		// ������
		select_server();
		// �Ҹ���
		virtual ~select_server();
		
		// �ʱ�ȭ �Լ�
		void initialize();
		// ����� ó�� �Լ�
		void finalize();
		// ����
		void run();
		// server�� Ȱ��ȭ�� ��� ���ǿ� ��ε� ĳ������ �Ѵ�.
		void broadcast(const char* msg, size_t size);
		// session�� ���� ���� ���� ��û
		// �ڷᱸ�� ��ȸ�� ���� ����� ������ �߻��� �� �ֱ� ������ ���� ó���� ��Ƽ� ó���Ѵ�.
		// �ߺ����� ȣ��ǵ� ���� 1ȸ�� ���� ó��.
		void request_disconnection(session* sess);
		// ���� ó���� ��Ƽ� �Ѵ�.
		void lazy_disconnect();

	private:
		// user ����� ��ȸ�ϸ鼭 update()�� ����.
		void update_logic();
		// core�� �ʱ�ȭ �Լ�
		bool initialize_core();
		// core�� ���� ó�� �Լ�
		void finalize_core();

		// select()�� ���ؼ� accept() ó���� �����ϴٸ� acceptó���� �Ѵ�.
		void try_accept();
		// select()�� ���ؼ� recv() ó���� ������ ������ recv ó���� �����Ѵ�.
		void try_receive_all_sessions();
		// send() ó���� ������ ����(=send_buffer�� �����Ͱ� �ִ� ����)�� send() ó���� �����Ѵ�.
		void try_send_all_sessions();
		// ���ο� ���ǿ� ���� accept() ó��.
		void accept_new_session();
		
		// session�� �Ҵ��ϰ� ����Ѵ�.
		void register_session_after_allocate(SOCKET sock);
		// session�� ��������Ѵ�.
		void unregister_session(session* sess);

	// ������Ʈ�� ���� ��� �޾Ƽ� �߰� ���� �� �Լ�.
	protected:
		// �߰� ������ contents �κп� ���� �ʱ�ȭȭ ���� ó�� �Լ�.
		virtual bool initialize_contents();
		virtual void finalize_contents();

		// session�� ���� �Ҵ�;
		virtual session* allocate_session() = 0;
		// user�� ���� �Ҵ� �Լ�.
		virtual i_user* allocate_user(const string& user_name) = 0;

		// session�� ���� �Ҵ� ������ ���
		virtual void free_session(session* sess) = 0;
		// user�� ���� �Ҵ� ������ ���
		virtual void free_user(i_user* user)     = 0;

		// session�� accept()�� ���� �߰� ó�� ���� �Լ�
		virtual void on_accept(session* sess) = 0;
		// session�� ����� ���� �߰� ó�� ���� �Լ�
		virtual void on_disconnect(session* sess) = 0;

	private:
		// accept()ó���� ��� �Ǵ� ����
		SOCKET								listen_sock;
		std::string							ip;
		uint16_t							port;
		// unique�� session_id ���� �����ø��� ����
		uint32_t							generated_session_id;

		// ������ ���� session�� ���� 
		unordered_map<SOCKET, session*>		sock_matching_table;
		// ���� ���� ���� ó���� ���� �Լ�.
		vector<session*>					to_disconnect_sessions;
	};

} // namespace core
} // namespace server
} // namespace c2
