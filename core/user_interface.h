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

	protected:
		session* get_session();

	private:
		session* owner;
	};
} // namespace core
} // namespace server
} // namespace c2
