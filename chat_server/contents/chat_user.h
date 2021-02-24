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

	public:
		// getter
		e_user_state get_state();
		string& get_name();
		chat_room* get_room() const;

		// setter
		void set_state(e_user_state);
		void set_name(string&& name);

		void enter_room(const string& room_name);
		void leave_room();
		void enter_lobby();
		void leave_lobby();
		void whisper(const std::string& user_name);
		

	private:
		std::string name;
		e_user_state state;
		chat_room* room;
	};
} // namespace contents
} // namespace server
} // namespace c2
