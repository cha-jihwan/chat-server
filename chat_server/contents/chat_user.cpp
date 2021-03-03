#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace contents 
{
	chat_user::chat_user() : room{}, state{e_user_state::US_NONE}
	{}

	chat_user::~chat_user() 
	{
	}
	
	
	void chat_user::update_logic()
	{}

	void chat_user::enter_room(const string& room_name)
	{
		leave_lobby();

		chat_room* room = g_room_manager->find_room_using_name(room_name);
		crash_if_false(nullptr != room);

		state = e_user_state::US_IN_ROOM;

		this->room = room;
		room->accept_user(this);
	}

	void chat_user::leave_room()
	{
		// 룸 나가는 처리
		room->export_user(this);

		// room에서 내가 방에 아무도 없는 경우 방 반납.
		if (0 == room->get_size())
		{
			g_room_manager->free(room);
		}
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

	void chat_user::set_name(const string& name)
	{
		this->name = name;
	}

} // namespace contents
} // namespace server
} // namespace c2
