#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace core 
{
	using namespace enumeration;

	session::session()
		: state{ e_session_state::INITIALIZED }
		, user{}, server{}
		, sock{ INVALID_SOCKET }
		, ip{}, port{}
	{}
	
	session::~session() {}
	
	void session::recv_payload()
	{
		int received_size = recv(sock, (char*)recv_buffer.get_write_buffer(), recv_buffer.get_buffer_size(), NULL);
		if (received_size == SOCKET_ERROR) 
		{
			// logging 필요
			printf("retval == socket_error %d\n", GetLastError());
		}
		else if (received_size == 0) // 끊긴거
		{
			printf("retval == 0 %d\n", GetLastError());
		}

		//crash_if_false();
		recv_buffer.move_write_head(received_size);
	}
	
	void session::parse_payload()
	{
		// 전역 packet 큐
		static vector<null_packet> packets;

		size_t parsed_size = i_parser<null_packet>::parse_payload(recv_buffer.get_header(), recv_buffer.get_use_size(), packets);
	}
	
	void session::send_payload()
	{
		int sent_size = send(sock, send_buffer.get_buffer(), send_buffer.get_use_size(), NULL);
		if (sent_size == SOCKET_ERROR)
		{
			// socket error 처리
		}

		send_buffer.move_read_head(sent_size);

		send_buffer.rewind();
	}
	
	size_t session::pre_send(const char* msg, size_t size)
	{
		size_t sent_size = send_buffer.write(const_cast<char*>(msg), size);
		if (sent_size != size)
		{
			printf("send buffer 사이즈 부족\n");
		}

		return sent_size;
	}
	
	SOCKET session::get_socket() const
	{
		return sock;
	}
	
	const string& session::get_ip() const
	{
		return ip;
	}
	
	const uint16_t session::get_port() const
	{
		return port;
	}
	
	const size_t session::send_buffer_size() const
	{
		return send_buffer.get_use_size();
	}
	
	i_user* session::get_user()
	{
		return nullptr;
	}

	
	void session::set_socket(SOCKET connected_sock)
	{
		this->sock = connected_sock;
	}

	
	void session::set_state(e_session_state state)
	{
		this->state = state;
	}

	
	void session::set_user(i_user* user)
	{
		crash_if_false(nullptr != user);

		this->user = user;
	}

	
	void session::set_server(select_server* in_server)
	{
		crash_if_false(nullptr != in_server);

		this->server = in_server;
	}





} // namespace core
} // namespace server
} // namespace c2


