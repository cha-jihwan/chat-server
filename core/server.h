#pragma once

namespace c2 { namespace server { namespace core 
{
	class select_server
	{
	public:
		select_server();
		virtual ~select_server();
		
		void initialize();
		void finalize();
		void run();

	private:
		void update_logic();
		bool initialize_core();
		void finalize_core();

		void try_accept();
		void try_receive_all_sessions();
		void try_send_all_sessions();

		void accept_new_session();
	protected:
		// broadcast
		// packet_type
		virtual bool initialize_contents();
		virtual void finalize_contents();

	private:
		SOCKET								listen_sock;
		std::string							ip;
		uint16_t							port;
		uint32_t							generated_session_id;

		unordered_map<SOCKET, session*>		sock_matching_table;
		//unordered_map<uint32_t, session*>	session_id_matching_table; 

		bounded_object_pool<session, constant::c_maximum_ccu>*	session_pool;
	};

} // namespace core
} // namespace server
} // namespace c2
