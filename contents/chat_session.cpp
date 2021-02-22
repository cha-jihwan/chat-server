#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents
{
	chat_session::chat_session() {}
	chat_session::~chat_session() {}
	void chat_session::parse_payload()
	{
		// Àü¿ª packet Å¥
		static vector<packet> packets;
		size_t parsed_size = telnet_parser::parse_payload(recv_buffer.get_header(), recv_buffer.get_use_size(), packets);
	}
	
} // namespace core
} // namespace server
} // namespace c2
