#include "pre_compile.h"

namespace c2 { namespace server { namespace core 
{
	using namespace enumeration;
	session::session() 
		: state{ e_session_state::INITIALIZED }
		, user{}, server{}
		, sock{ INVALID_SOCKET }
		, c_ip{}, c_port{}
	{}
	
	session::~session() {}
	
	void session::recv_payload()
	{
		int retval = recv(sock, (char*)recv_buffer.get_write_buffer(), recv_buffer.get_buffer_size(), NULL);
		if (retval == SOCKET_ERROR) // ����
		{
			// logging �ʿ�
			printf("retval == socket_error %d\n", GetLastError());
		}
		else if (retval == 0) // �����
		{
			printf("retval == 0 %d\n", GetLastError());
		}

		//crash_if_false();
		recv_buffer.move_write_head(retval);

		parse_payload(); 
	}

	void session::parse_payload()
	{
		for (;;)
		{
			size_t use_size = recv_buffer.get_use_size();
			if (use_size < 4) // �ּ� ��ɾ� �������� �ҷ��� �׳� 4�� �س���. 
			{
				break;
			}

			// �ּ� ��ɾ� ũ�� case 1: 1 + N + 2
			// case 1:'\'(1) + cmd(���� N) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
			// case 2:'\'(1) + cmd(���� N) + 1(sapce) + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
			// case 3:'\'(1) + cmd(���� N) + 1(sapce) + ���ID + SPACE + 2(Carage Return(0x0d), Line Feed(0x0a), \n)
			
			// case1 command \r\n
			// case2 command space body \r\n
			// case3 command space target_name body \r\n
			
			//char* header = recv_buffer.get_header();
			// \r\n �������� �и�.... buffer�� �Ѿ� ������?
			// space �������� �и�... buffer�� �Ѿ� ������?

			// 65536 ���ϸ� 
			//// recv_buffer.

			//recv_buffer.move_read_head(header->length);
		}

		recv_buffer.rewind();
	}

	void session::send_payload()
	{
		int ret_val = send(sock, send_buffer.get_buffer(), send_buffer.get_use_size(), NULL);
		if (ret_val == SOCKET_ERROR)
		{
			// socket error ó��
			// 
		}

		send_buffer.move_read_head(ret_val);
		send_buffer.rewind();
	}

	void session::request_send(const char* msg, size_t size)
	{
		size_t ret_val = send_buffer.write(const_cast<char*>(msg), size);
		if (ret_val != size)
		{
			printf("send buffer ������ ����\n");
		}
	}

	SOCKET session::get_socket() const
	{
		return sock;
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


	const string& session::get_ip() const
	{
		return c_ip;
	}
	const uint16_t session::get_port() const
	{
		return c_port;
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


