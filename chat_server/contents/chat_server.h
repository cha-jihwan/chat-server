#pragma once

namespace c2 { namespace server { namespace contents
{
	using namespace c2::server::core;

	// 채팅 서버.
	class chat_server : public select_server 
	{
	public:
		// 생성자
		chat_server();
		// 소멸자
		virtual ~chat_server() override final;

		// 채팅 서버에 필요한 초기화를 담당하는 함수.
		virtual bool initialize_contents();
		
		// user와 sesion에 대한 할당 함수.
		virtual session* allocate_session() override final;
		virtual i_user* allocate_user(const string& user_name) override final;

		// user와 session에 대한 할당 해제 함수.
		virtual void free_session(session* sess) override final; 
		virtual void free_user(i_user* user)     override final;

		// accpet 처리
		virtual bool on_accept(SOCKET sock)	override final;

		// session이 accept()된 이후의 콘텐츠단에서 필요한 처리
		virtual void on_session_join(session* sess)	override final;
		// session이 연결이 끊긴 이후의 콘텐츠단에서 필요한 처리
		virtual void on_disconnect(session* sess)	override final;

		// user 를 active_table에 추가.
		// 삽입이 되었다면 true를 리턴.
		bool register_user(const string& user_name, chat_user* user);

		// user 를 active_table에 추가.
		// 삽입이 되었다면 true를 리턴.
		void unregister_user(chat_user* user);

		// 이름을 통해 user를 찾는다.
		chat_user*	find_user_using_name(const string& name);

		// 현재 활성화된 유저들의 목록을 
		// "user_name\r\n" 포맷의 string객체를 통해 반환
		string		get_active_user_to_string();

		// kick thread에대한 처리.
		static void do_kick(HANDLE evnt, concurrency::concurrent_queue<SOCKET>* to_close_sockes);
		// server를 통해 호출하는 킥 요청..
		void request_kick_socket(SOCKET sock);


	private:
		// 현재 할당된 user의 name을 키로 하는 user객체 관리 테이블.
		unordered_map<string, chat_user*>							active_user_table;

		// session과 user의 오브젝트 풀.
		bounded_object_pool<chat_session, constant::c_maximum_ccu>*	session_pool;
		bounded_object_pool<chat_user, constant::c_maximum_ccu> *	user_pool;


		// 킥 처리를 위한 큐.
		concurrency::concurrent_queue<SOCKET>						to_kick_sockes;
		HANDLE														kick_event;
		thread*														kick_thread;
	};
} // namespace contents
} // namespace server
} // namespace c2
