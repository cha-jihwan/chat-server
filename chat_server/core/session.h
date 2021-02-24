#pragma once


namespace c2 { namespace server { namespace core
{
	using namespace enumeration;
	using namespace std;

	class i_user;
	class select_server;

	class session
	{
	public:
		session();
		virtual ~session();
		virtual void parse_payload();

		void recv_payload();
		void send_payload();
		size_t pre_send(const char* msg, size_t size);

		// getter
		SOCKET get_socket() const;
		const string& get_ip() const;
		const uint16_t get_port() const;
		const size_t send_buffer_size() const;
		i_user* get_user();
		e_session_state	get_state();


		// setter
		void set_socket(SOCKET connected_sock);
		void set_state(e_session_state state);
		void set_user(i_user* i_user);
		void set_server(select_server* server);

	protected:
		payload_buffer<65536>		recv_buffer;
		payload_buffer<65536>		send_buffer;
		SOCKET						sock;
		e_session_state				state;
		string						ip;
		uint16_t					port;

		i_user*						user;
		select_server*				server;
	};



} // namespace core
} // namespace server
} // namespace c2



