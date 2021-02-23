=======================================================================
# chat-server

ip : 127.0.0.1 (localhost)
port : 5500


구현된 기능 
echo server

TODO

core부 구현
- 세션 접속 처리 함수 분리.
- 세션 종료 처리

constents부 구현
- telnet_parser(명령어 분석 처리)
- 명령어에 따른 처리
- room
- user
- lobby
=======================================================================

메모장...

한명령어는 엔터로 구분
앞에 명령어가 없으면 잘못된 명령어로 무시 // 클라이게 알려는 줘야 함.

명령어는 아래의 3가지 종류
// case1: \command \r\n  
// [cmd][\r\n]
// case2: \command space body \r\n
// [cmd][space][body][\r\n]
// case3: \command space target_name body \r\n
// [cmd][space][target][space][body][\r\n]


로그인 기능     /LOGIN          /login      // case2  // login id
방생성 기능     /CREATE         /create     // case2  // create room_name
방 폭파         /DELETE         /delete     // case2  // delete romm_name
방 목록         /ROOMLIST       /roomlist   // case1
채팅            /CHAT           /chat       // case2 // chat 
귓속말          /WHISPER        /whiser     // case3
방 참여자 목록  /LOOKUPROOM     /            // case1
접속자 목록     /LOOCKUPALL     /lookupall   // case1

