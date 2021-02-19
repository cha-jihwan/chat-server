#pragma once

namespace server {  namespace core
{
	class session;
	class user_interface
	{
	public:
		void recv_payload();
		virtual ~user_interface();

	protected:
		session* get_session();

	private:
		session* owner;
	};
}
}
