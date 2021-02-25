#pragma once

namespace c2 { namespace server { namespace core
{
	// 소켓 주소 구조체를 래핑한 객체.
	// c#의 인터페이스 모방하여 제작.
	class end_point
	{
	public:
		// string 형태의 문자열 주소와 unsigned short 타입의 포트로 제작.
		end_point(const std::string& address, uint16_t c_port);
		// 생성자 기존 sockaddr_in 구조체를 통해 생성.
		end_point(const sockaddr_in& _sock_addr);
		
		// end_point 객체의 사이즈를 반환해준다.
		const size_t size() const;
		// sockaddr 구조체의 포인터로 캐스팅 후 반환해준다. 
		sockaddr* get_as_sockaddr();
		// sockaddr_in 구조체의 포인터로 캐스팅 후 반환해준다. 
		sockaddr_in* get_as_aockaddrin();
		
		// "ip:port" 형태의 string 객체로 반환해준다.
		const std::string to_string();
		// port를 unsigned short 형태로 반환해준다.
		const uint16_t port();
		// ip를 문자열 형태로 string객체로 반환해준다.
		std::string ip();

	private:
		sockaddr sock_addr;
	};

}// namespace core
}// namespace server
}// namespace c2
