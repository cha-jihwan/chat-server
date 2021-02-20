#pragma once

#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include <cstdint>
#include <string>
//#include <string_view> for vs15
#include <array>
#include <vector>
#include <unordered_map>


// core
#include "environment.h"
#include "protocol_core.h"
#include "crash.h"
#include "object_pool.hpp"
#include "payload_buffer.h"

#include "session.h"          // user_interface ��ȣ ����
#include "user_interface.h"	  // session ��ȣ ����

#include "server.h"
// contents


