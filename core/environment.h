#pragma once



namespace server { namespace core
{
	namespace constant
	{
		constexpr uint16_t	port		{	};
		constexpr uint16_t	ip			{	};
		constexpr size_t	maximum_size{	};
	} // namespace constant

	namespace global
	{
		//constexpr uint16_t port{	};
	} // namespace global

	namespace enumeration
	{
		enum class session_state
		{
			initialized,
			established,
			closed,
			state_max
		};

		enum server_core_error
		{
		};
	} // namespace enumeration

} // namespace core
} // namespace server
