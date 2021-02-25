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
		virtual i_user* allocate_user(const string& user_name) override final;

		virtual void free_session(session* sess) override final; 
		virtual void free_user(i_user* user)     override final;

		virtual void on_accept(session* sess)	override final;
		virtual void on_disconnect(session* sess)	override final;

		// user 를 active_table에 추가.
		// 삽입이 되었다면 true를 리턴.
		bool register_user(const string& user_name, chat_user* user);

		// user 를 active_table에 추가.
		// 삽입이 되었다면 true를 리턴.
		void unregister_user(chat_user* user);

		chat_user*	get_user(const string& name);
		string		get_active_user_to_string();

	private:
		unordered_map<string, chat_user*>		active_user_table;

		bounded_object_pool<chat_session, constant::c_maximum_ccu>*	session_pool;
		bounded_object_pool<chat_user, constant::c_maximum_ccu> *	user_pool;
	};
} // namespace contents
} // namespace server
} // namespace c2
