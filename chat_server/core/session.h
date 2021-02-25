#pragma once


namespace c2 { namespace server { namespace core
{
	using namespace enumeration;
	using namespace std;

	// ���� ����
	class i_user;
	class select_server;

	class session
	{
	public:
		// ������
		session();
		// �Ҹ���.
		virtual ~session();

		// �����Լ� session ���� ��ӹ޾Ƽ� �ش� ������Ʈ�� ���缭 �����Ѵ�.
		virtual void parse_payload();

		// session�� ���� ���ۿ� ���� �Ѵ�.
		void recv_payload();
		
		// session�� �۽� ���ۿ� �ִ� �����͸� ���� �����Ѵ�.
		void send_payload();

		// session�� �۽� ���ۿ� �����͸� ���� �ִ´�.
		size_t pre_send(const char* msg, size_t size);

		// getter
		SOCKET get_socket() const;
		const string& get_ip() const;
		const uint16_t get_port() const;
		const size_t send_buffer_size() const;
		i_user* get_user();
		e_session_state	get_state();
		select_server*	get_server();

		// setter
		void set_socket(SOCKET connected_sock);
		void set_state(e_session_state state);
		void set_user(i_user* i_user);
		void set_server(select_server* server);
		void set_port(uint16_t port);
		void set_ip(string&& ip);


	protected:
		// ���� ���� �����͸� �޾Ƽ� ��Ŷ ������ ���� ���
		payload_buffer<65536>		recv_buffer;
		// �۽� ���� �����͸� ��Ƽ� ������ ���� ���
		payload_buffer<65536>		send_buffer;
		SOCKET						sock;
		// ������ ����.
		e_session_state				state;
		string						ip;
		uint16_t					port;
		// ������ �������̽� user�� �Ҵ�� ���� 
		i_user*						user;
		// ������ �������̽� �ַ� ������ �Ҵ�� ���� setter�� ���� ������ ����.
		select_server*				server;
	};



} // namespace core
} // namespace server
} // namespace c2



