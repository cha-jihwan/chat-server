#pragma once

#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <cstdint>
#include <string>
//#include <string_view> 
#include <array>
#include <vector>


// core
#include "environment.h"
#include "protocol_core.h"
#include "crash.h"	 		
#include "object_pool.h"
#include "payload_buffer.h"

#include "session.h"          // user_interface 상호 참조
#include "user_interface.h"	  // session 상호 참조

#include "server.h"
// contents


