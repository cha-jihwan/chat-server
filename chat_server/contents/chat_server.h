#pragma once

namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core;

	// ä�� ����.
	class chat_server : public select_server 
	{
	public:
		// 
		chat_server();
		virtual ~chat_server();

		// ä�� ������ �ʿ��� �ʱ�ȭ�� ����ϴ� �Լ�.
		virtual bool initialize_contents();
		
		// user�� sesion�� ���� �Ҵ� �Լ�.
		virtual session* allocate_session() override final;
		virtual i_user* allocate_user(const string& user_name) override final;

		// user�� session�� ���� �Ҵ� ���� �Լ�.
		virtual void free_session(session* sess) override final; 
		virtual void free_user(i_user* user)     override final;

		// session�� accept()�� ������ �������ܿ��� �ʿ��� ó��
		virtual void on_accept(session* sess)	override final;
		// session�� ������ ���� ������ �������ܿ��� �ʿ��� ó��
		virtual void on_disconnect(session* sess)	override final;

		// user �� active_table�� �߰�.
		// ������ �Ǿ��ٸ� true�� ����.
		bool register_user(const string& user_name, chat_user* user);

		// user �� active_table�� �߰�.
		// ������ �Ǿ��ٸ� true�� ����.
		void unregister_user(chat_user* user);

		// 
		chat_user*	get_user(const string& name);

		// ���� Ȱ��ȭ�� �������� ����� 
		// "user_name\r\n" ������ string��ü�� ���� ��ȯ
		string		get_active_user_to_string();

	private:
		// ���� �Ҵ�� user�� name�� Ű�� �ϴ� user��ü ���� ���̺�.
		unordered_map<string, chat_user*>		active_user_table;

		// session�� user�� ������Ʈ Ǯ.
		bounded_object_pool<chat_session, constant::c_maximum_ccu>*	session_pool;
		bounded_object_pool<chat_user, constant::c_maximum_ccu> *	user_pool;
	};
} // namespace contents
} // namespace server
} // namespace c2
