#pragma once

namespace c2 { namespace server {  namespace core
{
	// ���� ����
	class session;

	// user�� ���� �������̽� Ŭ����.
	// ��ӹ޾Ƽ� ����
	class i_user
	{
	public:
		// ������
		i_user();
		// �Ҹ���
		virtual ~i_user();

		// update class 
		virtual void update_logic();

		// getter
		session* get_session();

		// setter
		void	 set_session(session* sess);
	private:
		// session�� ���� ������ Ŀ�ø��Ǿ� �ִ�.
		session* owner;
	};

} // namespace core
} // namespace server
} // namespace c2
