#include "pre_compile.h"

namespace server { namespace core
{
	using namespace enumeration;
	session::session() 
		: state{ session_state::initialized }
		, user{}, owner{}
		, sock{ INVALID_SOCKET }
		, ip{}, port{}
	{
	}

	session::~session()
	{
	}
	
	void session::parse_payload()
	{
	}

	SOCKET session::get_socket()
	{
		return sock;
	}
	void session::set_socket(SOCKET connected_sock)
	{
	}
	const string& session::get_ip() const
	{
		// TODO: 여기에 return 문을 삽입합니다.
	}
	const uint16_t session::get_port() const
	{
		return uint16_t();
	}
} // namespace core
} // namespace server


