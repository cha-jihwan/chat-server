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
			printf("received_size == socket_error %d\n", GetLastError());
	
			server->request_disconnection(this);
		}
		else if (received_size == 0) // �����
		{
			server->request_disconnection(this);

			printf("recv err-no %d\n", GetLastError());
		}

		recv_buffer.move_write_head(received_size);
	}
	
	
	// �����Լ�... ����� ���� session ���� �ٸ��� ó���� �Ѵ�.
	void session::parse_payload()
	{
		// example
		// ���� packet ť
		// static vector<null_packet> packets;

		// packets.clear();

		// size_t parsed_size = i_parser<null_packet>::parse_payload(recv_buffer.get_header(), recv_buffer.get_use_size(), packets);
	}
	

	void session::send_payload()
	{
		int sent_size = send(sock, send_buffer.get_buffer(), send_buffer.get_use_size(), NULL);
		if (sent_size == SOCKET_ERROR)
		{
			// socket error ó��
			server->request_disconnection(this);
		}

		send_buffer.move_read_head(sent_size);

		send_buffer.rewind();
	}
	
	size_t session::pre_send(const char* msg, size_t size)
	{
		size_t sent_size = send_buffer.write(const_cast<char*>(msg), size);
		if (sent_size != size)
		{
			printf("send buffer ������ ����\n");
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

	e_session_state session::get_state()
	{
		return this->state;
	}

	select_server* session::get_server()
	{
		return server;
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


