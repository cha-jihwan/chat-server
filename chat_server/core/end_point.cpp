#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace core
{
	//end_point::end_point(uint32_t address, uint16_t port)
	//{
	//	((sockaddr_in*)&sock_addr)->sin_family = AF_INET;
	//	((sockaddr_in*)&sock_addr)->sin_port = htons(port);
	//	((sockaddr_in*)&sock_addr)->sin_addr.s_addr = htonl(address);
	//}

	end_point::end_point(const std::string& address, uint16_t c_port)
	{
		get_as_aockaddrin()->sin_family = AF_INET;
		get_as_aockaddrin()->sin_port = htons(c_port);
		InetPtonA(AF_INET, address.c_str(), &get_as_aockaddrin()->sin_addr);
	}

	//end_point::end_point(const wstring & address, uint16_t port)
	//{
	//	((sockaddr_in*)&sock_addr)->sin_family = AF_INET;
	//	((sockaddr_in*)&sock_addr)->sin_port = htons(port);
	//	InetPtonW(AF_INET, address.c_str(), &((sockaddr_in*)&sock_addr)->sin_addr);
	//}

	end_point::end_point(const sockaddr_in& _sock_addr)
	{
		memcpy(&sock_addr, &_sock_addr, sizeof(sockaddr));
	}

	std::string end_point::ip()
	{
		char addr_str_buf[32]{};
		return ::inet_ntop(AF_INET, reinterpret_cast<sockaddr_in*>(((char*)&sock_addr.sa_data) + 2), addr_str_buf, sizeof(addr_str_buf));
	}

	////
	const std::string end_point::to_string()
	{
		return std::string(this->ip()) + ":" + std::to_string(this->port());
	}


	const uint16_t end_point::port()
	{
		return ::ntohs(reinterpret_cast<sockaddr_in*>(&sock_addr)->sin_port);
	}

	sockaddr* end_point::get_as_sockaddr()
	{
		return &sock_addr;
	}

	sockaddr_in* end_point::get_as_aockaddrin()
	{
		return reinterpret_cast<sockaddr_in*>(&sock_addr);
	}

	const size_t end_point::size() const
	{
		return sizeof(this->sock_addr);
	}

}// namespace core
}// namespace server
}// namespace c2