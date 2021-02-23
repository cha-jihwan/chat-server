#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace core
{
	select_server::select_server() 
		: listen_sock{ INVALID_SOCKET }
		, ip{}, port{}
		, generated_session_id{}
	{}

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
		// timer�� �ʿ��Ѱ�...
		for (;;)
		{
			try_accept(); // accept ó��
			try_receive_all_sessions(); // recv ó��
			update_logic();
			try_send_all_sessions(); // send ó��
		}
	}

	void select_server::initialize()
	{
		printf("server initialized..");

		// ���� �ʱ�ȭ �� core�ܿ��� �ʿ��� �� �ʱ�ȭ
		if (false == initialize_core())
		{
			printf("select_server::initialize_core() ");
			crash();
		}

		printf("..");

		// �������ܿ��� �ʿ��� �� �ʱ�ȭ
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

		return true;
	}

	void select_server::finalize_core()
	{
		// ���� ���� ����.

		// linger opt
		closesocket(listen_sock);

		// winsockt lib ����
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

	i_user * select_server::allocate_user()
	{
		return nullptr;
	}

	void select_server::free_session(session * sess)
	{
	}

	void select_server::free_user(i_user * user)
	{
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
		// �ʱ�ȭ
		timeval tv{0, 0};
		FD_SET read_set;
		read_set.fd_count = 0;

		// FD_SET�� ��´�.
		for (const auto& kv : sock_matching_table)
		{
			read_set.fd_array[read_set.fd_count] = kv.first;
			read_set.fd_count += 1;
		}

		// select()�� ���� recv() ȣ���� ������ ������ ã�� ���ٸ� �ٷ� �����Ѵ�.
		if (1 <= select(read_set.fd_count, &read_set, nullptr, nullptr, &tv))
		{
			for (const auto& kv : sock_matching_table)
			{
				if (FD_ISSET(kv.first, &read_set))
				{
					session* sess = kv.second;

					crash_if_false(nullptr != sess);

					// session�� recv()�� ȣ�� �� ó��
					sess->recv_payload();
					sess->parse_payload();
				}
			}
		}
		else
		{
			// SOCKET_ERROR �̸� ����.
			// 0�̸� recv() ȣ���� ������ ����.
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
		// accept ó��
		SOCKADDR_IN sock_addr{};
		int addr_len = sizeof(sock_addr);
		SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&sock_addr, &addr_len);
		if (SOCKET_ERROR == client_sock)
		{
			printf("accept() error : %d all new session()\n", GetLastError());
		}


///////////////////////////////
		// �̱��� : kick ó��
		if ( constant::c_maximum_ccu <= sock_matching_table.size() )
		{
			// kick ó��...
			// thread ���� ó��....
			// 

			return;
		}
///////////////////////////////


		// �ű� session ó�� 
		// �Լ��� ������ ������...
		session* sess = this->allocate_session();

		crash_if_false(nullptr != sess);

		sess->set_state(e_session_state::ESTABLISHED);
		sess->set_socket(client_sock);
		sess->set_server(this);

		sock_matching_table.emplace(client_sock, sess);
	}

	void select_server::release_session(SOCKET sock)
	{
		// session ����
	}

} // namespace core
} // namespace server
} // namespace c2
