#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents
{
	chat_session::chat_session() {}
	chat_session::~chat_session() {}
	void chat_session::parse_payload()
	{
		static vector<packet> packets;
		
		size_t parsed_size = telnet_parser::parse_payload(this, recv_buffer.get_header(), recv_buffer.get_use_size(), packets);

		recv_buffer.move_read_head(parsed_size);
		
		recv_buffer.rewind();
	}
} // namespace core
} // namespace server
} // namespace c2
