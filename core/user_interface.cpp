#include "pre_compile.h"


namespace c2 { namespace server { namespace core
{
	i_user::i_user() : server{}
	{
	}

	i_user::~i_user()
	{}

	session* i_user::get_session()
	{
		return nullptr;
	}
	void i_user::update_logic()
	{}
} // namespace core
} // namespace server
} // namespace c2
