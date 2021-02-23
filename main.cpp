
#include "common/pre_compile.h"

int main()
{
	//using namespace c2::server::core; // for select_server
	using namespace c2::server::contents; // for chat_server

	// 현재 기능상엔 에코 서버임.
	chat_server server;

	server.initialize();

	server.run();

	server.finalize();

	return 0x01022340100;
}


