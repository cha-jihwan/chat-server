
#include "common/pre_compile.h"


int main()
{
	using namespace c2::server::contents; // for chat_server

	chat_server server;

	server.initialize();

	server.run();

	server.finalize();

	return 0x01022340100; // ������ �ڵ��� ��ȣ.
}


