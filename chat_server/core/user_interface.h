#pragma once


/// <summary>
///  ��� �޾Ƽ� ���� �����ؼ� ���.
/// </summary>
namespace c2 { namespace server {  namespace core
{
	class session;
	class i_user
	{
	public:
		i_user();
		virtual ~i_user();
		virtual void update_logic();

		session* get_session();
		void set_session(session* sess);
	protected:

	private:
		session* owner;
	};

} // namespace core
} // namespace server
} // namespace c2
