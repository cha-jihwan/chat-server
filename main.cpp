
#include "common/pre_compile.h"

int main()
{
	using namespace c2::server::core;

	select_server server;

	server.initialize();

	server.run();

	server.finalize();

	return 0x01022340100;
}


