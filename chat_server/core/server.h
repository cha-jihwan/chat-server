#pragma once

namespace c2 { namespace server { namespace core 
{
	// server의 base class 
	// 상속 받고 가상함수를 통해 추가 기능을 구현한다.
	class select_server
	{
	public:
		// 생성자
		select_server();
		// 소멸자
		virtual ~select_server();
		
		// 초기화 함수
		void initialize();
		// 종료시 처리 함수
		void finalize();
		// 시작
		void run();
		// server의 활성화된 모든 세션에 브로드 캐스팅을 한다.
		void broadcast(const char* msg, size_t size);
		// session에 대한 연결 끊기 요청
		// 자료구조 순회중 연결 끊기시 문제가 발생할 수 있기 때문에 종료 처리를 모아서 처리한다.
		// 중복으로 호출되도 최초 1회만 해제 처리.
		void request_disconnection(session* sess);
		// 종료 처리를 모아서 한다.
		void lazy_disconnect();

	private:
		// user 목록을 순회하면서 update()를 실행.
		void update_logic();
		// core단 초기화 함수
		bool initialize_core();
		// core단 종료 처리 함수
		void finalize_core();

		// select()를 통해서 accept() 처리가 가능하다면 accept처리를 한다.
		void try_accept();
		// select()를 통해서 recv() 처리가 가능한 세션은 recv 처리를 유도한다.
		void try_receive_all_sessions();
		// send() 처리가 가능한 세션(=send_buffer에 데이터가 있는 세션)은 send() 처리를 유도한다.
		void try_send_all_sessions();
		// 새로운 세션에 대한 accept() 처리.
		void accept_new_session();
		
		// session을 할당하고 등록한다.
		void register_session_after_allocate(SOCKET sock);
		// session을 등록해제한다.
		void unregister_session(session* sess);

	// 프로젝트에 따라서 상속 받아서 추가 구현 할 함수.
	protected:
		// 추가 구현된 contents 부분에 대한 초기화화 종료 처리 함수.
		virtual bool initialize_contents();
		virtual void finalize_contents();

		// session에 대한 할당;
		virtual session* allocate_session() = 0;
		// user에 대한 할당 함수.
		virtual i_user* allocate_user(const string& user_name) = 0;

		// session에 대한 할당 해제를 담당
		virtual void free_session(session* sess) = 0;
		// user에 대한 할당 해제를 담당
		virtual void free_user(i_user* user)     = 0;

		virtual bool on_accept(SOCKET sock) = 0;
		// session이 accept()된 이후 추가 처리 구현 함수
		virtual void on_join(session* sess) = 0;
		// session이 종료된 이후 추가 처리 구현 함수
		virtual void on_disconnect(session* sess) = 0;

		// getter
		size_t get_ccu();
	private:
		// accept()처리에 사용 되는 소켓
		SOCKET								listen_sock;
		std::string							ip;
		uint16_t							port;
		// unique한 session_id 세션 생성시마다 증가
		uint32_t							generated_session_id;

		// 소켓을 통해 session을 관리 
		unordered_map<SOCKET, session*>		sock_matching_table;
		// 늦은 연결 종료 처리를 위한 함수.
		vector<session*>					to_disconnect_sessions;
	};

} // namespace core
} // namespace server
} // namespace c2
