#pragma once

namespace c2 { namespace server { namespace core
{
	class end_point
	{
	public:
		//end_point(uint32_t address, uint16_t port);
		end_point(const std::string& address, uint16_t c_port);
		//end_point(const wstring& address, uint16_t port);
		//end_point(const sockaddr& _sock_addr);
		
		const size_t size() const;
		sockaddr* get_as_sockaddr();
		sockaddr_in* get_as_aockaddrin();
		
		const std::string to_string();
		const uint16_t port();
		std::string ip();

	private:
		sockaddr sock_addr;
	};

}// namespace core
}// namespace server
}// namespace c2


		//template <class T>
		//endpoint(T&& address, uint16_t port)
		//{
		//	((sockaddr_in*)&sock_addr)->sin_family = AF_INET;
		//	((sockaddr_in*)&sock_addr)->sin_port = htons(port);

		//	if constexpr (std::is_same<std::string, T>::value)
		//	{
		//		InetPtonA(AF_INET, address.c_str(), &((sockaddr_in*)&sock_addr)->sin_addr);
		//	}
		//	else if constexpr (std::is_same<std::wstring, T>::value)
		//	{
		//		InetPtonW(AF_INET, address.c_str(), &((sockaddr_in*)&sock_addr)->sin_addr);
		//	}
		//	else // w st 
		//	{
		//		InetPtonW(AF_INET, address, &((sockaddr_in*)&sock_addr)->sin_addr);
		//	}
		//}


		//template <class T>
		//endpoint(T&& address, uint16_t port)
		//{
		//	((sockaddr_in*)&sock_addr)->sin_family = AF_INET;
		//	((sockaddr_in*)&sock_addr)->sin_port = htons(port);

		//	if constexpr (std::is_same<std::string_view, T>::value)
		//	{
		//		InetPtonA(AF_INET, address.c_str(), &((sockaddr_in*)&sock_addr)->sin_addr);
		//	}
		//	else if constexpr (std::is_same<std::wstring_view, T>::value)
		//	{
		//		InetPtonW(AF_INET, address.c_str(), &((sockaddr_in*)&sock_addr)->sin_addr);
		//	}
		//}
