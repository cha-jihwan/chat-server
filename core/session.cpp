#include "pre_compile.h"

namespace c2 { namespace server { namespace core 
{
	using namespace enumeration;
	session::session() 
		: state{ e_session_state::INITIALIZED }
		, user{}, owner{}
		, sock{ INVALID_SOCKET }
		, ip{}, port{}
	{}

	session::~session()
	{}
	
	void session::recv_payload()
	{
		int retval = recv(sock, (char*)recv_buffer.get_write_buffer(), recv_buffer.get_buffer_size(), NULL);
		if (retval == SOCKET_ERROR) // 문제
		{
			// logging 필요
			printf("\n%d\n\n", GetLastError());
		}
		else if (retval == 0) // 끊긴거
		{
			printf("\n%d\n\n", GetLastError());
		}

		//crash_if_false();
		recv_buffer.move_write_head(retval);

		parse_payload(); 
	}

	void session::parse_payload()
	{

	}

	void session::send_payload()
	{
		int retval = send(sock, send_buffer.get_buffer(), send_buffer.get_use_size(), NULL);
		if (retval == SOCKET_ERROR)
		{
			//err_display("send()");
			//RemoveSocketInfo(i);
		}

		send_buffer.move_read_head(retval);
		send_buffer.rewind();
	}

	SOCKET session::get_socket() const
	{
		return sock;
	}
	
	void session::set_socket(SOCKET connected_sock)
	{
	}

	void session::set_state(e_session_state state)
	{
	}

	void session::set_user(i_user* user)
	{
		crash_if_false(nullptr != user);

		this->user = user;
	}


	const string& session::get_ip() const
	{
		// TODO: 여기에 return 문을 삽입합니다.
	}
	const uint16_t session::get_port() const
	{
		return uint16_t();
	}
	const size_t session::send_buffer_size() const
	{
		return send_buffer.get_use_size();
	}
	i_user* session::get_user()
	{
		return nullptr;
	}

} // namespace core
} // namespace server
} // namespace c2


