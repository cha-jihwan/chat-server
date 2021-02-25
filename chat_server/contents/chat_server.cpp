#include "../common/pre_compile.h"
#include "chat_server.h"


namespace c2 { namespace server { namespace contents 
{
	chat_server::chat_server() 
		: session_pool{}, user_pool{}
		, kick_event{ INVALID_HANDLE_VALUE }
		, kick_thread{ nullptr }
	{
	}	  

	chat_server::~chat_server()  {}

	bool chat_server::initialize_contents()
	{
		telnet_parser::initialize();
		
		session_pool = new bounded_object_pool<chat_session, constant::c_maximum_ccu>();
		user_pool    = new bounded_object_pool<chat_user, constant::c_maximum_ccu>();

		g_lobby = new chat_lobby();
		g_room_manager = new chat_room_manager();


		// kick event 알림을 위한 evnet 객체
		kick_event = CreateEvent(NULL, TRUE, FALSE, NULL);
		// kick 전용 스레드 생성.
		kick_thread = new std::thread{ do_kick, kick_event, &to_kick_sockes };

		return true;
	}

	session* chat_server::allocate_session()
	{
		chat_session* chatting_session = session_pool->allocate(); 
		
		crash_if_false(nullptr != chatting_session);
	
		return chatting_session;
	}

	i_user* chat_server::allocate_user(const string& user_name)
	{
		chat_user* chatting_user= user_pool->allocate();

		crash_if_false(nullptr != chatting_user);

		chatting_user->set_name(user_name);

		register_user(user_name, chatting_user);

		return chatting_user;
	}


	void chat_server::free_session(session* sess)
	{
		crash_if_false(nullptr != sess);

		session_pool->free((chat_session*)sess);
	}

	void chat_server::free_user(i_user* user)
	{
		crash_if_false(nullptr != user);

		unregister_user((chat_user*)user);

		user_pool->free((chat_user*)user);
	}

	bool chat_server::on_accept(SOCKET sock)
	{
		size_t ccu = this->get_ccu();
		if (ccu + 1 > c_maximum_ccu)
		{
			request_kick_socket(sock);
			return false;
		}

		return true;
	}

	void chat_server::on_session_join(session* sess)
	{
		using namespace c2::server::contents;

		crash_if_false(nullptr != sess);

		printf("%s:%d session established....\r\n", sess->get_ip().c_str(), sess->get_port());

		sess->pre_send(gui_header, sizeof(gui_header));
		sess->pre_send(end_msg, 2);
		sess->pre_send(gui_body, sizeof(gui_body));
		sess->pre_send(end_msg, 2);
	}

	void chat_server::on_disconnect(session* sess)
	{
		crash_if_false(nullptr != sess);

		printf("%s:%d session logout....\r\n", sess->get_ip().c_str(), sess->get_port());

		// 유저 종료 처리.
		e_session_state state = sess->get_state();
		if (e_session_state::LOGINED <= state)
		{
			chat_user* user = (chat_user*)sess->get_user();
			crash_if_false(nullptr != user);
			unregister_user(user);
		}
	}

	bool chat_server::register_user(const string& user_name, chat_user* user)
	{
		crash_if_false(nullptr != user);

		// 삽입이 되었다면 true를 리턴
		return active_user_table.emplace(user_name, user).second;
	}

	void chat_server::unregister_user(chat_user* user)
	{
		crash_if_false(nullptr != user);
		
		active_user_table.erase(user->get_name());
	}

	chat_user* chat_server::find_user_using_name(const string& name)
	{
		chat_user* user = active_user_table[name];
		
		return user;
	}

	string chat_server::get_active_user_to_string()
	{
		string active_user_list;

		for (auto& kv : active_user_table)
		{
			active_user_list += kv.first;
			active_user_list += "\r\n";
		}

		return std::move(active_user_list);
	}

	void chat_server::do_kick(HANDLE evnt, concurrency::concurrent_queue<SOCKET>* to_kick_sockes)
	{
		printf("start kick thread....\n");

		for (;;)
		{
			size_t ret = WaitForSingleObject(evnt, INFINITE);
			if (ret == WAIT_FAILED) //HANDLE이 Invalid 할 경우
			{
				LOG("kcik_event WAIT_FAILED\r\n");
				break;
			}
			if (ret == WAIT_TIMEOUT) //TIMEOUT시 명령
			{
				continue;
			}
			else // 깨어나면 가지고 있는 모든 소켓에 대해서 shutdown 처리
			{
				SOCKET sock;
				while(true == to_kick_sockes->try_pop(sock))
				{
					// send 이후 
					if (sizeof(kick_message) != send(sock, kick_message, sizeof(kick_message), NULL))
					{
						LOG("sent size err-no : %d \r\n", GetLastError());
					}

					// 종료 유도..
					shutdown(sock, SD_SEND);
					printf("동접을 초과한 유저를 처리했습니다. \r\n");
				}
			}

			ResetEvent(evnt); //Event Object Nonsignaled 상태로 변환
		}

		printf("stop kick thread....\n");
	}

	void chat_server::request_kick_socket(SOCKET sock)
	{
		to_kick_sockes.push(sock);
		
		SetEvent(kick_event);
	}

} // namespace contents
} // namespace server
} // namespace c2