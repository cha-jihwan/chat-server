#pragma once

namespace c2 { namespace server { namespace contents
{
	class chat_server : public c2::server::core::select_server
	{
		chat_server();
		virtual ~chat_server();
	};

} // namespace contents
} // namespace server
} // namespace c2
