#pragma once


/// <summary>
///  상속 받아서 로직 구현해서 사용.
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
