#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents 
{
	chat_server::chat_server()
	{}

	chat_server::~chat_server()
	{}

	bool chat_server::initialize_contents()
	{
		// 
		//session_pool = new un

		return true;
	}
	session * chat_server::allocate_session()
	{
		return nullptr;
	}
	i_user * chat_server::allocate_user()
	{
		return nullptr;
	}
	void chat_server::free_session(session * sess)
	{
	}
	void chat_server::free_user(i_user * user)
	{
	}
} // namespace contents
} // namespace server
} // namespace c2