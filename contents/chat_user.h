#pragma once




namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core;

	class chat_room;
	class chat_lobby;
	class chat_user : public i_user
	{
	public:
		chat_user();
		virtual ~chat_user();
		virtual void update_logic();

	private:
		// void enter_room();
		// void leave_room();
		// void explode_room();
		// void whisper(const std::strin& user_nickname);

		// room
		// lobby
	};
} // namespace contents
} // namespace server
} // namespace c2
