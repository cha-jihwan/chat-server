#pragma once

namespace c2 { namespace server { namespace contents
{
	class chat_server : public c2::server::core::select_server
	{
	public:
		chat_server();
		virtual ~chat_server();


		virtual bool initialize_contents();
		virtual session* allocate_session() override final;
		virtual i_user* allocate_user()     override final;

		virtual void free_session(session* sess) override final; 
		virtual void free_user(i_user* user)     override final;

	private:
		bounded_object_pool<session, constant::c_maximum_ccu>*	session_pool;
		bounded_object_pool<i_user, constant::c_maximum_ccu>*	user_pool;
	};
} // namespace contents
} // namespace server
} // namespace c2
