#pragma once

namespace c2 { namespace server { namespace contents
{
	class chat_room
	{
	public:
		chat_room();
		~chat_room();
	private:
		std::vector<session*> ss;
	};

} // namespace contents
} // namespace server
} // namespace c2