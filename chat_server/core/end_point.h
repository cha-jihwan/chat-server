#pragma once

namespace c2 { namespace server { namespace core
{
	// ���� �ּ� ����ü�� ������ ��ü.
	// c#�� �������̽� ����Ͽ� ����.
	class end_point
	{
	public:
		// string ������ ���ڿ� �ּҿ� unsigned short Ÿ���� ��Ʈ�� ����.
		end_point(const std::string& address, uint16_t c_port);
		// ������ ���� sockaddr_in ����ü�� ���� ����.
		end_point(const sockaddr_in& _sock_addr);
		
		// end_point ��ü�� ����� ��ȯ���ش�.
		const size_t size() const;
		// sockaddr ����ü�� �����ͷ� ĳ���� �� ��ȯ���ش�. 
		sockaddr* get_as_sockaddr();
		// sockaddr_in ����ü�� �����ͷ� ĳ���� �� ��ȯ���ش�. 
		sockaddr_in* get_as_aockaddrin();
		
		// "ip:port" ������ string ��ü�� ��ȯ���ش�.
		const std::string to_string();
		// port�� unsigned short ���·� ��ȯ���ش�.
		const uint16_t port();
		// ip�� ���ڿ� ���·� string��ü�� ��ȯ���ش�.
		std::string ip();

	private:
		sockaddr sock_addr;
	};

}// namespace core
}// namespace server
}// namespace c2
