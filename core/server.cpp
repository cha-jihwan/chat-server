#include "pre_compile.h"

namespace server { namespace core
{
	select_server::select_server() 
		: listen_sock{ INVALID_SOCKET }
		, ip{}, port{}
		, generated_session_id{}
	{}

	select_server::~select_server()
	{
	}

	void select_server::run()
	{
		// timer가 필요한가...
		for (;;)
		{
			// recv 
				// session.parse_payload();
					// processing
			recv_payload();
			// send
			send_payload();
		}
	}

	void select_server::initialize()
	{
		// init


		if (false == initialize_contents())
		{
			crash();
		}
	}

	void select_server::finalize()
	{
		
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
			//err_quit("socket()");
			return false;
		}


		// bind()
		SOCKADDR_IN serveraddr{};
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
		serveraddr.sin_port = htons(server::core::constant::port);

		int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR)
		{
			//err_quit("bind()");
			return false;
		}


		// listen()
		retval = listen(listen_sock, SOMAXCONN);
		if (retval == SOCKET_ERROR)
		{
			//err_quit("listen()");
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
	}

	bool select_server::initialize_contents()
	{
		return false;
	}

	void select_server::finalize_contents()
	{}

	void select_server::recv_payload()
	{
		// 
		// select(,);
	}

	void select_server::send_payload()
	{
	}

} // namespace core
} // namespace server

