#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents
{
	chat_session::chat_session() {}
	chat_session::~chat_session() {}
	void chat_session::parse_payload()
	{
		size_t sent_size = pre_send(recv_buffer.get_buffer(), recv_buffer.get_use_size());
		
		printf("sent : size %d \n", sent_size);

		recv_buffer.move_read_head(sent_size);
		
		recv_buffer.rewind();
		//// Àü¿ª packet Å¥
		//static vector<packet> packets;
		//size_t parsed_size = telnet_parser::parse_payload(recv_buffer.get_header(), recv_buffer.get_use_size(), packets);
	}
	
	void chat_session::enter_room(string room_name)
	{
	}

	void chat_session::leave_room()
	{
	}

	void chat_session::enter_lobby()
	{
	}

	void chat_session::leave_lobby()
	{
	}
	
} // namespace core
} // namespace server
} // namespace c2
