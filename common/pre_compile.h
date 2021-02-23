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

// util
#include "../util/crash.h"

// net

// core
#include "../core/protocol.h"
#include "../core/environment.h"
#include "../core/end_point.h"
#include "../core/object_pool.hpp"
#include "../core/payload_buffer.h"

#include "../core/parser_interface.h"
#include "../core/session.h"          // user_interface 상호 참조
#include "../core/user_interface.h"	  // session 상호 참조

#include "../core/server.h"

// contents
#include "../contents/contents_text.h"
#include "../contents/telnet_parser.h"
#include "../contents/chat_user.h"
#include "../contents/chat_session.h"
#include "../contents/room.h"
#include "../contents/lobby.h"
#include "../contents/chat_server.h"

