#pragma once

namespace c2 { namespace server { namespace contents
{
	class lobby
	{
	public:
		lobby();
		~lobby();

	private:
		std::unordered_map<std::string, chat_session*> named_users;
	};

	extern lobby* g_lobby;

} // namespace contents
} // namespace server
} // namespace c2