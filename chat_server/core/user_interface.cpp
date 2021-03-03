#include "../common/pre_compile.h"


namespace c2 { namespace server { namespace core
{
	
	i_user::i_user() : owner{}
	{}

	i_user::~i_user()
	{}
	
	session* i_user::get_session()
	{
		return this->owner;
	}

	void i_user::set_session(session * sess)
	{
		this->owner = sess;
	}
	
	void i_user::update_logic()
	{}


} // namespace core
} // namespace server
} // namespace c2
