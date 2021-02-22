#pragma once

namespace c2 { namespace server { namespace contents
{
	class chat_session : public c2::server::core::session
	{
	public:
		chat_session();
		virtual ~chat_session();
		virtual void parse_payload();
	};


} // namespace contents
} // namespace server
} // namespace c2