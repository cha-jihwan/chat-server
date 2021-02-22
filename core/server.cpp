#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace core
{
	select_server::select_server() 
		: listen_sock{ INVALID_SOCKET }
		, c_ip{}, c_port{}
		, generated_session_id{}
	{}

	select_server::~select_server()
	{}

	void select_server::update_logic()
	{
		for (auto& kv : sock_matching_table)
		{
			kv.second->get_user()->update_logic();
		}
	}

	void select_server::run()
	{
		// timer가 필요한가...
		for (;;)
		{
			// accept 처리
			try_accept();
			// recv 
				// session.parse_payload();
					// processing
			try_receive_all_sessions();
			// 
			update_logic();
			// send
			try_send_all_sessions();
		}
	}

	void select_server::initialize()
	{
		// 윈속 초기화
		if (false == initialize_core())
		{
			printf("select_server::initialize_core() ");
			crash();
		}

		if (false == initialize_contents())
		{
			printf("select_server::initialize_contents() ");
			crash();
		}
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
		//SOCKADDR_IN serveraddr{ AF_INET, htons(server::core::constant::port),  };
		//serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
		int retval = bind(listen_sock, end_point.get_as_sockaddr(), (int)end_point.size());
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

		session_pool = new bounded_object_pool<session, constant::c_maximum_ccu>();

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

	bool select_server::initialize_contents()
	{
		return false;
	}

	void select_server::finalize_contents() {}

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
		timeval tv{};
		FD_SET read_set;
		read_set.fd_count = 0;


		for (const auto& kv : sock_matching_table)
		{
			read_set.fd_array[read_set.fd_count] = kv.first;
			read_set.fd_count += 1;
		}

		if (1 <= select(read_set.fd_count, &read_set, nullptr, nullptr, &tv))
		{
			for (const auto& kv : sock_matching_table)
			{
				if (FD_ISSET(kv.first, &read_set))
				{
					session* sess = kv.second;

					crash_if_false(nullptr != sess);

					sess->recv_payload();
					//sess->parse_payload();
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

		}


///////////////////////////////
		// 미구현 : kick 처리
		if ( constant::c_maximum_ccu <= sock_matching_table.size() )
		{
			// kick 처리...
			// thread 만들어서 처리....
			// 

			return;
		}

		
		session* sess = session_pool->allocate();
		crash_if_false(nullptr != sess);

		sess->set_state(e_session_state::ESTABLISHED);
		sess->set_socket(client_sock);
		// ip, port
		// sesion을 끄내고 
		// session 추가
		// user 할당 X
		// 신규 처리
	}

	void select_server::release_session(SOCKET sock)
	{
		// session 제거
	}

} // namespace core
} // namespace server
} // namespace c2
