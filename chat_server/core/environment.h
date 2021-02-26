#pragma once

// �ھ�ܿ��� ����ϴ� ȯ�� ����.
namespace c2 { namespace server { namespace core
{
	// core�ܿ��� ��� ����
	namespace constant
	{
		extern const char*		c_ip;//{ "127.0.0.1" };
		constexpr uint16_t		c_port			{ 5500 };
		constexpr size_t		c_maximum_ccu	{ 4 };//{ 64 };
		constexpr size_t		c_maximum_size	{ 4 };
	} // namespace constant

	// core�ܿ��� ���� ����
	namespace global
	{
	} // namespace global

	namespace enumeration
	{
		// session�� ������ ������ Ÿ��
		enum class e_session_state
		{
			INITIALIZED, 
			CLOSED,		// 
			ESTABLISHED,// ���� ��, user �Ҵ� ��
			LOGINED,	// user �Ҵ� ����
			STATE_MAX,
		};

		enum server_core_error
		{};
	} // namespace enumeration

} // namespace core
} // namespace server
} // namespace c2


// �������ܿ��� ����ϴ� ȯ�� ����
namespace c2 { namespace server { namespace core
{

	// contents�ܿ��� ��� ����
	namespace constant
	{
		constexpr size_t	c_cmd_size	{ 2 };
		constexpr size_t	c_crlf_size	{ 2 };
	} // namespace constant

	namespace global
	{
		//constexpr uint16_t port{	};
	} // namespace global

	namespace enumeration
	{
		// user�� ������ ������ Ÿ��.
		enum class e_user_state
		{
			US_NONE,
			US_IN_LOBBY,
			US_IN_ROOM,
			US_MAX,
		};


	} // namespace enumeration

} // namespace core
} // namespace server
} // namespace c2
