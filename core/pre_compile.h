#pragma once

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <cstdint>
#include <string>
//#define STRSAFE_DEPRECATE  
//#include <strsafe.h>  
//#include <string_view> for 
#include <array>
#include <vector>
#include <unordered_map>


// core
#include "crash.h"
#include "environment.h"
#include "protocol_core.h"
#include "end_point.h"
#include "object_pool.hpp"
#include "payload_buffer.h"

#include "session.h"          // user_interface ��ȣ ����
#include "user_interface.h"	  // session ��ȣ ����

#include "server.h"
// contents


