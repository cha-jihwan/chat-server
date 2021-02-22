#pragma once




namespace c2 { namespace server { namespace contents
{
	class chat_room;
	class chat_lobby;
	class chat_user : public c2::server::core::i_user
	{
	public:
		chat_user();
		virtual ~chat_user();
		virtual void update_logic();
	};
} // namespace contents
} // namespace server
} // namespace c2
