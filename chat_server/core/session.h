#pragma once


namespace c2 { namespace server { namespace core
{
	using namespace enumeration;
	using namespace std;

	// 전방 선언
	class i_user;
	class select_server;

	class session
	{
	public:
		// 생성자
		session();
		// 소멸자.
		virtual ~session();

		// 가상함수 session 마다 상속받아서 해당 프로젝트에 맞춰서 구현한다.
		virtual void parse_payload();

		// session의 수신 버퍼에 수신 한다.
		void recv_payload();
		
		// session의 송신 버퍼에 있는 데이터를 전부 전송한다.
		void send_payload();

		// session의 송신 버퍼에 데이터를 집어 넣는다.
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
		// 수신 버퍼 데이터를 받아서 패킷 조립을 위해 사용
		payload_buffer<65536>		recv_buffer;
		// 송신 버퍼 데이터를 모아서 보내기 위해 사용
		payload_buffer<65536>		send_buffer;
		SOCKET						sock;
		// 세션의 상태.
		e_session_state				state;
		string						ip;
		uint16_t					port;
		// 유저의 인터페이스 user가 할당된 이후 사용된다.
		i_user*						user;
		// 서버의 인터페이스 주로 세션이 할당된 이후 setter를 통해 의존성 주입.
		select_server*				server;
	};

} // namespace core
} // namespace server
} // namespace c2



