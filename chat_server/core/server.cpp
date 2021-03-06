#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace core
{
	select_server::select_server() 
		: listen_sock{ INVALID_SOCKET }
		, ip{}, port{}
		, generated_session_id{}
	{
		// 최대 사용 크기 미리 예약
		// 재할당을 막는다.
		to_disconnect_sessions.reserve(c_maximum_ccu);
	}

	select_server::~select_server() {}

	void select_server::update_logic()
	{
		for (auto& kv : sock_matching_table)
		{
			if (nullptr != kv.second->get_user())
			{
				kv.second->get_user()->update_logic();
			}
		}
	}

	void select_server::run()
	{
		printf("start...\n");

		
		for (;;)
		{
			// accept 처리
			try_accept();				
			// recv 처리
			try_receive_all_sessions(); 
			update_logic();
			// 모든 보낼 데이터가 있는 세션에 대한 send 처리.
			try_send_all_sessions();	
			// 모든 처리 작업이 끝난 이후 세션 종료 처리.
			lazy_disconnect();			
		}
	}

	void select_server::initialize()
	{
		// setup dump
		SetUnhandledExceptionFilter(exception_filter);

		printf("server initialized..");

		// 윈속 초기화 및 core단에서 필요한 것 초기화
		if (false == initialize_core())
		{
			printf("select_server::initialize_core() ");
			crash();
		}

		printf("..");

		// 컨텐츠단에서 필요한 것 초기화
		if (false == initialize_contents())
		{
			printf("select_server::initialize_contents() ");
			crash();
		}

		printf("OK\n");
	}

	void select_server::finalize()
	{
		finalize_contents();
		finalize_core();
	}

	bool select_server::initialize_core()
	{
		// init winsock 
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			printf("WSAStartup() error in select_server err-code : %d", GetLastError());
			return false;
		}


		listen_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (listen_sock == INVALID_SOCKET)
		{
			printf("socket() error in select_server err-code : %d", GetLastError());
			return false;
		}

		// bind()
		end_point end_point(constant::c_ip , constant::c_port);
		int retval = ::bind(listen_sock, end_point.get_as_sockaddr(), (int)end_point.size());
		if (retval == SOCKET_ERROR)
		{
			printf("bind() error in initialize_core() err-code : %d", GetLastError());
			return false;
		}

		// listen()
		retval = listen(listen_sock, SOMAXCONN);
		if (retval == SOCKET_ERROR)
		{
			printf("listen() error in initialize_core() err-code : %d", GetLastError());
			return false;
		}

		// sock opt
		// linger 
		// nagle

		return true;
	}

	void select_server::finalize_core()
	{
		// 소켓 연결 끊기.

		// linger opt
		closesocket(listen_sock);

		// winsockt lib 종료
		WSACleanup();
	}

	void select_server::broadcast(const char * msg, size_t size)
	{
		for (const auto& kv : sock_matching_table)
		{
			if (size != kv.second->pre_send(msg, size))
			{
				// error handling
			}
		}
	}

	bool select_server::initialize_contents()
	{
		return false;
	}

	void select_server::finalize_contents() {}

	session * select_server::allocate_session()
	{
		return nullptr;
	}

	i_user* select_server::allocate_user(const string& user_name)
	{
		return nullptr;
	}

	void select_server::try_accept()
	{
		FD_SET read_set{ 1, {this->listen_sock} };
		timeval tv{0, 0};

		int ret_val = select(1, &read_set, nullptr, nullptr, &tv);
		switch (ret_val)
		{
			case 0: break;
			case 1:
			{
				accept_new_session();
				break;
			}
			default:
			{
				printf("select() error : %d in try_accept()\n", GetLastError());
				break;
			}
		}
	}

	void select_server::try_receive_all_sessions()
	{
		// 초기화
		timeval tv{0, 0};
		FD_SET read_set;
		read_set.fd_count = 0;

		// FD_SET에 담는다.
		for (const auto& kv : sock_matching_table)
		{
			read_set.fd_array[read_set.fd_count] = kv.first;
			read_set.fd_count += 1;
		}

		// select()를 통해 recv() 호출이 가능한 소켓을 찾고 없다면 바로 리턴한다.
		if (1 <= select(read_set.fd_count, &read_set, nullptr, nullptr, &tv))
		{
			for (const auto& kv : sock_matching_table)
			{
				if (FD_ISSET(kv.first, &read_set))
				{
					session* sess = kv.second;

					crash_if_false(nullptr != sess);

					// session의 recv()를 호출 및 처리
					sess->recv_payload();
					sess->parse_payload();
				}
			}
		}
		else
		{
			// SOCKET_ERROR 이면 에러.
			// 0이면 recv() 호출할 세션이 없다.
		}
	}

	void select_server::try_send_all_sessions()
	{
		for (const auto& kv : sock_matching_table)
		{
			if (0 < kv.second->send_buffer_size())
			{
				kv.second->send_payload();
			}
		}
	}

	void select_server::accept_new_session()
	{
		// accept 처리
		SOCKADDR_IN sock_addr{};
		int addr_len = sizeof(sock_addr);
		SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&sock_addr, &addr_len);
		if (SOCKET_ERROR == client_sock)
		{
			printf("accept() error : %d all new session()\n", GetLastError());
		}

		// 상속된 서버에서 처리.
		if (false == on_accept(client_sock))
		{
			return;
		}

		// sesion 할당 및 세션 세팅
		// 서버에 가입.
		register_session_after_allocate(client_sock);
	}

	void select_server::request_disconnection(session* sess) 
	{
		to_disconnect_sessions.push_back(sess);
	}

	//
	void select_server::lazy_disconnect()
	{
		for (session* sess : to_disconnect_sessions)
		{
			if (e_session_state::CLOSED != sess->get_state())
			{
				unregister_session(sess);
			}
		}

		to_disconnect_sessions.clear();
	}

	// 신규 session 처리 
	void select_server::register_session_after_allocate(SOCKET connected_sock)
	{
		session* sess = this->allocate_session();
		crash_if_false(nullptr != sess);


		sess->set_state(e_session_state::ESTABLISHED);
		sess->set_socket(connected_sock);
		sess->set_server(this);


		sockaddr_in sock_addr{};
		int sock_addr_len = sizeof(sock_addr);
		getsockname(connected_sock, (sockaddr*)&sock_addr, &sock_addr_len);

		end_point client_addr(sock_addr);
		sess->set_port( client_addr.port());
		sess->set_ip(std::move(client_addr.ip()));



		sock_matching_table.emplace(connected_sock, sess);

		on_session_join(sess);
	}

	// session 종료 처리 
	void select_server::unregister_session(session* sess)
	{
		crash_if_false(nullptr != sess);

		on_disconnect(sess);

		// 로그인 상태라면?
		// user 반납.
		if (e_session_state::LOGINED <= sess->get_state())
		{
			chat_user* user = (chat_user*)sess->get_user();

			crash_if_false(nullptr != user);

			this->free_user(user);
		}

		sess->set_state(e_session_state::CLOSED);

		sock_matching_table.erase(sess->get_socket());

		this->free_session(sess);
	}

	// 가상 함수 인터페이스 들...
	void select_server::free_session(session* sess) {}
	void select_server::free_user(i_user* user) {}
	bool select_server::on_accept(SOCKET sock) { return false; }
	void select_server::on_session_join(session* sess) {}
	void select_server::on_disconnect(session* sess) {}

	size_t select_server::get_ccu()
	{
		return sock_matching_table.size();
	}

} // namespace core
} // namespace server
} // namespace c2
