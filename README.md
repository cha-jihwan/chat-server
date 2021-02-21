# chat-server

메모장...

한명령어는 엔터로 구분
앞에 명령어가 없으면 잘못된 명령어로 무시 // 클라이게 알려는 줘야 함.

명령어는 아래의 3가지 종류
// case1: \command \r\n
// case2: \command space body \r\n
// case3: \command space target_name body \r\n


로그인 기능 /LOGIN /login
방생성 기능 /CREATE /create
방 폭파        /DELETE /delete
방 목록        /SELECT /select
채팅             /CHAT /chat
귓속말         /WHISPER /whiser
방 참여자 목록 /LOOKUPROOM /lookuproom
접속자 목록  /LOOCKUPALL /lookupall
