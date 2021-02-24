#include "../common/pre_compile.h"

namespace c2 { namespace server { namespace contents
{
	chat_lobby* g_lobby{};

	chat_lobby::chat_lobby()
	{
	}

	chat_lobby::~chat_lobby()
	{
	}

	void chat_lobby::broadcast(char * msg, size_t msg_size)
	{
		for (const auto& kv : named_users)
		{
			crash_if_false(nullptr != kv.second);
			crash_if_false(nullptr != kv.second->get_session());

			if (msg_size != kv.second->get_session()->pre_send(msg, msg_size))
			{
				// error handling
			}
		}
	}

	//void lobby::accept_user(chat_user* user)
	//{
	//	this->named_users.emplace(user->get_name(), user);
	//}


} // contents
} // server
} // c2