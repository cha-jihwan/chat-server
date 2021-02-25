#pragma once

namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core;

	class chat_session : public session
	{
	public:
		chat_session();
		virtual ~chat_session();
		virtual void parse_payload();
		
	};

} // namespace contents
} // namespace server
} // namespace c2
