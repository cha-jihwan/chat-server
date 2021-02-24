#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace contents 
{
	chat_user::chat_user() : room{}
	{}
	chat_user::~chat_user() {}
	
	
	void chat_user::update_logic()
	{}

	void chat_user::enter_room(const string & room_name)
	{
		leave_lobby();

		// room name ���� �� ã��
		// nullptr üũ
		state = e_user_state::US_IN_ROOM;

		// ���� unordered_map
	}

	void chat_user::leave_room()
	{
		state = e_user_state::US_NONE;

		//room->export_user(this);
		// room���� ���� �濡 �ƹ��� ���� ��� �� �ݳ�.
	}

	void chat_user::enter_lobby()
	{
		state = enumeration::e_user_state::US_IN_LOBBY;

		g_lobby->accept_user(this);
	}

	
	void chat_user::leave_lobby()
	{
		state = e_user_state::US_NONE;

		g_lobby->export_user(this);
	}

	void chat_user::whisper(const std::string & user_name)
	{

	}

	e_user_state chat_user::get_state()
	{
		return state;
	}

	string& chat_user::get_name()
	{
		return this->name;
	}

	chat_room* chat_user::get_room() const
	{
		return room;
	}

	void chat_user::set_state(e_user_state new_state)
	{
		this->state = new_state;
	}

	void chat_user::set_name(string&& name)
	{
		this->name = name;
	}

} // namespace contents
} // namespace server
} // namespace c2
