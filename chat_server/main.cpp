
#include "common/pre_compile.h"


int main()
{
	using namespace c2::server::contents; // for chat_server

	LOG("hi\n");

	chat_server server;

	server.initialize();

	server.run();

	server.finalize();

	return 0x01022340100; // 관리자 핸드폰 번호.
}


