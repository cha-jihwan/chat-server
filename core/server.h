#pragma once

namespace server { namespace core {
	class select_server
	{
	public:
		select_server();

		virtual ~select_server();
		void run();
		
		void initialize();
		void finalize();

	private:
		bool initialize_core();
		void finalize_core();

		virtual bool initialize_contents();
		virtual void finalize_contents();

		void recv_payload();
		void send_payload();
		// broadcast
		// packet_type
	private:
		SOCKET		listen_sock;
		std::string ip;
		uint16_t	port;

		uint32_t	generated_session_id;
	};

} // namespace core
} // namespace server
