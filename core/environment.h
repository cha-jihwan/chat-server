#pragma once

namespace c2 { namespace server { namespace core
{
	namespace constant
	{
		extern const char*		c_ip;//{ "127.0.0.1" };
		constexpr uint16_t		c_port			{ 5500 };
		constexpr size_t		c_maximum_ccu	{ 64 };
		constexpr size_t		c_maximum_size	{ 64 };
	} // namespace constant

	namespace global
	{
		//constexpr uint16_t port{	};
	} // namespace global

	namespace enumeration
	{
		enum class e_session_state
		{
			INITIALIZED, 
			ESTABLISHED,// ���� ��, user �Ҵ� ��
			LOGINED,	// user �Ҵ� ����
			CLOSED,		// 
			STATE_MAX,
		};

		enum server_core_error
		{};
	} // namespace enumeration

} // namespace core
} // namespace server
} // namespace c2
