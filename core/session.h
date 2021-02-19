#pragma once


namespace server { namespace core 
{
	using namespace enumeration;
	using namespace std;
	class user_interface;
	class select_server;
	class session
	{
	public:
		session();
		~session();

		void parse_payload();

		// getter
		SOCKET get_socket();
		const string& get_ip() const;
		const uint16_t get_port() const;
		
		// setter
		void set_socket(SOCKET connected_sock);

	private:
		payload_buffer<65536>	recv_buffer;
		payload_buffer<65536>	send_buffer;
		session_state			state;
		user_interface*			user;
		select_server*			owner;
		string					ip;
		uint16_t				port;
		SOCKET					sock;
	};

} // namespace core
} // namespace server



