#pragma once

namespace c2 { namespace server {  namespace core
{
	// 전방 선언
	class session;

	// user에 대한 인터페이스 클래스.
	// 상속받아서 구현
	class i_user
	{
	public:
		// 생성자
		i_user();
		// 소멸자
		virtual ~i_user();

		// update class 
		virtual void update_logic();

		// getter
		session* get_session();

		// setter
		void	 set_session(session* sess);
	private:
		// session에 대한 포인터 커플링되어 있다.
		session* owner;
	};

} // namespace core
} // namespace server
} // namespace c2
