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
			CLOSED,		// 
			ESTABLISHED,// 연결 후, user 할당 전
			LOGINED,	// user 할당 이후
			STATE_MAX,
		};

		enum server_core_error
		{};
	} // namespace enumeration

} // namespace core
} // namespace server
} // namespace c2



namespace c2 { namespace server { namespace core
{
	namespace global
	{
		//constexpr uint16_t port{	};
	} // namespace global

	namespace enumeration
	{
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
