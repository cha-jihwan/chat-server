#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace contents 
{
	chat_user::chat_user() {}
	chat_user::~chat_user() {}
	void chat_user::update_logic()
	{
		// 구현 바람.
	}

	void chat_user::enter_room(const string & room_name)
	{
	}

	void chat_user::leave_room()
	{
	}

	void chat_user::enter_lobby()
	{
		state = enumeration::e_user_state::US_IN_LOBBY;


	}

	void chat_user::leave_lobby()
	{
	}

	void chat_user::whisper(const std::string & user_name)
	{
	}

	e_user_state chat_user::get_state()
	{
		return state;
	}

	void chat_user::set_state(e_user_state)
	{
	}

} // namespace contents
} // namespace server
} // namespace c2
