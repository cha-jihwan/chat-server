#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace contents 
{
	chat_server::chat_server()
	{}

	chat_server::~chat_server() 
	{}

	bool chat_server::initialize_contents()
	{
		session_pool = new bounded_object_pool<chat_session, constant::c_maximum_ccu>();
		user_pool    = new bounded_object_pool<chat_user, constant::c_maximum_ccu>();

		return true;
	}

	session* chat_server::allocate_session()
	{
		chat_session* chatting_session = session_pool->allocate(); 
		
		crash_if_false( nullptr != chatting_session );
	
		return chatting_session;
	}

	i_user * chat_server::allocate_user()
	{
		chat_user* chatting_user= user_pool->allocate();

		crash_if_false(nullptr != chatting_user);

		return chatting_user;
	}


	void chat_server::free_session(session* sess)
	{
		crash_if_false(nullptr != sess);

		session_pool->free((chat_session*)sess);
	}

	void chat_server::free_user(i_user * user)
	{
		crash_if_false(nullptr != user);

		user_pool->free((chat_user*)user);
	}
} // namespace contents
} // namespace server
} // namespace c2