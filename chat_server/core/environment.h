#pragma once

// 코어단에서 사용하는 환경 변수.
namespace c2 { namespace server { namespace core
{
	// core단에서 상수 변수
	namespace constant
	{
		extern const char*		c_ip;//{ "127.0.0.1" };
		constexpr uint16_t		c_port			{ 5500 };
		constexpr size_t		c_maximum_ccu	{ 2 };//{ 64 };
		constexpr size_t		c_maximum_size	{ 64 };
	} // namespace constant

	// core단에서 전역 변수
	namespace global
	{
	} // namespace global

	namespace enumeration
	{
		// session의 상태의 열거형 타입
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


// 콘텐츠단에서 사용하는 환경 변수
namespace c2 { namespace server { namespace core
{
	namespace global
	{
		//constexpr uint16_t port{	};
	} // namespace global

	namespace enumeration
	{
		// user의 상태의 열거형 타입.
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
