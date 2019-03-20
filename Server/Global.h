#ifndef GLOBAL_H
#define GLOBAL_H

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>
#include <math.h>
#include <queue>
#include <list>
#include <ctime>
#include "Vector3.h"

// 패킷 : [전체크기 4Byte][프로토콜 4Byte][데이터]
// 문자열 데이터 [strlen(문자열)+1 4Byte][문자열]
enum PROTOCOL {
	EXIT = -1,
	CLIENT_JOIN_MENU_CHOICE = 0,			// 회원가입 메뉴 선택			[프로토콜][int]
	CLIENT_LOGIN_MENU_CHOICE,				// 로그인 메뉴 선택				[프로토콜][int]
	CLIENT_REQ_EXIT_JOIN,					// 회원가입 취소 선택			[프로토콜]
	CLIENT_REQ_ID_OVERLAP_CHECK,			// 회원가입 ID중복체크 요청		[프로토콜][int][char *]
	CLIENT_REQ_JOIN,						// 회원가입 요청				[프로토콜][int][char *]
	CLIENT_REQ_LOGIN,						// 로그인 요청					[프로토콜][int][char *][int][char *]

	SERVER_JOIN,							// 회원가입 메뉴 선택			[프로토콜]
	SERVER_LOGIN,							// 로그인 메뉴 선택				[프로토콜]
	SERVER_EXIT_JOIN,						// 회원가입 취소 결과			[프로토콜]
	SERVER_ID_OVERLAP_CHECK,				// ID중복 결과 보냄				[프로토콜][bool]
	SERVER_JOIN_SUCCESS,					// 회원가입 결과 메세지			[프로토콜]
	SERVER_LOGIN_SUCCESS,					// 로그인 성공 여부				[프로토콜][bool]

	CLIENT_REQ_CHARACTER_SLOT,				// 클라 케릭터 슬롯 요청		[프로토콜]
	CLIENT_CHARACTER_ENTER,					// 클라 케릭터 접속				[프로토콜][int]
	CLIENT_CHARACTER_DELETE,				// 클라 케릭터 삭제				[프로토콜][int]
	CLIENT_NEW_CHARACTER_MENU,				// 클라 케릭터생성 메뉴			[프로토콜]
	CLIENT_REQ_NICK_OVERLAP_CHECK,			// 클라 닉네임 중복				[프로토콜][int][char*]
	CLIENT_REQ_CHARACTER,					// 클라 케릭터 생성 요청		[프로토콜]
	CLIENT_CHARACTER_EXIT,					// 클라 케릭터 생성 취소		[프로토콜]
	CLIENT_LOGOUT,							// 클라 로그아웃				[프로토콜]
	CLIENT_LEAVE,							// 클라 회원탈퇴				[프로토콜]

	SERVER_CHARACTER_SLOT_RESULT,			// 서버 케릭터 슬롯 전송		[프로토콜]...
	SERVER_CHARACTER_ENTER_RESULT,			// 서버 케릭터 접속결과			[프로토콜][bool]
	SERVER_CHARACTER_ENTER_INFO,			// 서버 케릭터 접속 정보		[DataProtocol용]
	SERVER_CHARACTER_DELETE_RESULT,			// 서버 케릭터 삭제 결과		[프로토콜][bool]
	SERVER_CHARACTER_MENU,					// 서버 케릭터 메뉴 결과		[프로토콜]
	//SERVER_NICK_OVERLAP_CHECK,			// 서버 닉네임 중복 결과		[프로토콜][bool]
	SERVER_CHARACTER_RESULT,				// 서버 케릭터 생성 확인 결과	[프로토콜][bool]
	SERVER_CHARACTER_EXIT_RESULT,			// 서버 케릭터 생성 취소 결과	[프로토콜[
	SERVER_LEAVE,							// 서버 회원탈퇴 결과			[프로토콜][bool]
	SERVER_LOGOUT,							// 서버 로그아웃 결과			[프로토콜][bool]

	SERVER_TENDER,							// 입찰 선택
	CLIENT_TENDER_MENU_CHOICE,				// 입찰메뉴 선택 확인
	SERVER_TENDER_CREATE,					// 입찰물품 생성 선택 
	SERVER_TENDER_PARTICIPATE,				// 입찰 참여 선택
	SERVER_BACK,							// 뒤로가기
	CLIENT_REQ_TENDER_CREATE,				// 입찰물품 생성 요청	 [프로토콜][char* 물품이름][int 가격][int 입찰자수]
	SERVER_TENDER_CREATE_RESULT,			// 입찰물품 생성 요청 결과
	CLIENT_REQ_TENDER_LIST,					// 입찰 리스트 요청
	SERVER_TENDER_LIST_RESULT,				// 입찰 리스트 요청 결과
	CLIENT_REQ_TENDER_PARTICIPATE,			// 입찰 요청
	SERVER_TENDER_PARTICIPATE_RESULT,		// 입찰 요청 결과
	SERVER_TENDER_END_RESULT,				// 입찰 종료 결과 [프로토콜][char* 메세지]

	CLIENT_INGAME_OTHERPLAYERLIST,			// 클라 다른플레이어 리스트 요청	[프로토콜][]
	CLIENT_INGAME_MOVE_START,				// 클라 이동 시작					[프로토콜]
	CLIENT_INGAME_MOVE_REPORT,				// 클라 이동중 보고					[프로토콜]
	CLIENT_INGAME_MOVE_END,					// 클라 이동 끝					[프로토콜]
	CLIENT_INGAME_MOVE_ROTATION,			// 클라 회전
	SERVER_INGAME_MOVE_ORDER,				// 서버 이동 명령				[프로토콜][FLOAT][FLOAT][FLOAT][FLOAT : 시간]
	SERVER_INGAME_MOVE_RESULT,				// 서버 이동 결과				[프로토콜][FLOAT][FLOAT][FLOAT][FLOAT : 시간]
	SERVER_INGAME_MOVE_OTHERPLAYERINFO,		// 서버 다른플레이어 이동정보			[프로토콜][FLOAT][FLOAT][FLOAT][FLOAT : 시간][닉네임사이즈][닉네임]
	SERVER_INGAME_MOVE_ROTATION,			// 클라 회전
	SERVER_INGAME_OTHERPLAYERLIST_RESULT,	// 서버 플레이어 리스트			[프로토콜][INT:카운트][INT:닉네임사이즈][CHAR:닉네임][VECTOR:위치]...
	SERVER_INGAME_OTHERPLAYER_CONNECT,		// 서버 플레이어 접속 정보

	CLIENT_INGAME_MENU_REQ_CHARACTER,		// 클라 캐릭터 선택화면으로 요청
	CLIENT_INGAME_MENU_REQ_LOGOUT,			// 클라 로그아웃
	CLIENT_INGAME_MENU_EXIT,				// 클라 게임종료
	SERVER_INGAME_MENU_RESULT_CHARACTER,	// 서버 캐릭터 선택화면 결과
	SERVER_INGAME_MENU_RESULT_LOGOUT,		// 서버 로그아웃 결과
	SERVER_INGAME_OTHERPLAYER_LEAVE,		// 서버 다른 유저가 인게임에서 나갈때

	CLIENT_INGAME_CHANNEL_INFO,				// 클라 채널 정보 요청
	CLIENT_INGAME_CHANNEL_CHANGE,			// 클라 채널 이동 요청
	SERVER_INGAME_CHANNLE_INFO_RESULT,		// 서버 채널 정보 결과
	SERVER_INGAME_CHANNLE_CHANGE_RESULT,	// 서버 채널 이동 요청 결과
	SERVER_INGAME_CHANNLE_USER_CHANGE,		// 서버 채널 이동 요청시 그 유저 이동했다고 통보

	CLIENT_INGAME_PARTY_ROOM_CREATE,		// 클라 파티방 생성 요청
	CLIENT_INGAME_PARTY_ROOM_INFO,			// 클라 파티방 정보 요청
	CLIENT_INGAME_PARTY_ROOM_JOIN,			// 클라 파티 참여 요청
	CLIENT_INGAME_PARTY_ROOM_INVITE,		// 클라 파티 초대 요청
	CLIENT_INGAME_PARTY_USER_KICK,			// 클라 파티원 강퇴 요청
	CLIENT_INGAME_PARTY_USER_LEAVE,			// 클라 파티 탈퇴 요청
	SERVER_INGAME_PARTY_ROOM_CREATE_RESULT,	// 서버 파티방 생성 결과
	SERVER_INGAME_PARTY_ROOM_INFO_RESULT,	// 서버 파티방 정보
	SERVER_INGAME_PARTY_ROOM_JOIN_RESULT,	// 서버 파티 참여 결과
	SERVER_INGAME_PARTY_ROOM_INVITE,		// 서버 파티 초대(특정유저에게)
	SERVER_INGAME_PARTY_ROOM_INVITE_RESULT,	// 서버 파티 초대 결과
	SERVER_INGAME_PARTY_USER_KICK,			// 서버 파티 강퇴
	SERVER_INGAME_PARTY_USER_KICK_RESULT,	// 서버 파티 강퇴결과
	SERVER_INGAME_PARTY_USER_LEAVE_RESULT,	// 서버 파티 탈퇴결과
	SERVER_INGAME_PARTY_USER_INFO,			// 서버 파티원정보(파티원숫자,닉네임,HP,MP)

	// 채팅 프로토콜
	CLIENT_REQ_CHAT_LIST,					// 현재 로그인한 회원리스트 요청 			 프로토콜만
	SERVER_CHAT_LIST_RESULT,				// 현재 로그인한 회원리스트 요청 결과 		[인원 수] [닉네임 사이즈] [닉네임]...
	CLIENT_REQ_CHAT_START,					// 채팅 요청 								[인원 수] [닉네임 사이즈] [닉네임]
	SERVER_CHAT_START_RESULT,				// 채팅 요청 결과 							[BOOL값]  ?  [방 번호] : [에러 코드] [메세지 사이즈] [메세지]
	SERVER_CHAT_MSG,						// 채팅 메세지 : 서버 --> 클라				[방 번호] [메세지 사이즈] [메세지]
	CLIENT_CHAT_MSG,						// 채팅 메세지 : 클라 --> 서버				[방 번호] [메세지 사이즈] [메세지]
	SERVER_CHAT_ENTER,						// 현재 채팅방 입장 : 서버 --> 클라			[방 번호] [닉네임 사이즈] [닉네임]
	CLIENT_CHAT_EXIT,						// 현재 채팅방 퇴장 : 클라 --> 서버			[방 번호] [닉네임 사이즈] [닉네임]
	SERVER_CHAT_EXIT						// 채팅방 퇴장 : 서버 --> 클라				[방 번호]
};


enum STATE {
	INIT_STATE, LOGIN_STATE, TENDER_STATE, JOIN_STATE, EXIT_STATE,
	LOG_IN,
	LOG_OUT,
	TENDER_INTRO,

	CLOSE = -1,
	RECV_LOGOUT_MENU_CHOICE = 0,
	RECV_LOGIN_MENU_CHOICE,
	SEND_JOIN,
	RECV_REQ_ID_OVERLAP_CHECK,
	SEND_ID_OVERLAP_CHECK,
	RECV_REQ_JOIN,
	SEND_JOIN_SUCCESS,
	SEND_LOGIN,
	RECV_REQ_LOGIN,
	SEND_LOGIN_SUCCESS,
	SEND_LOGIN_FAILED,
	SEND_LEAVE,
	SEND_LOGOUT,

	SEND_TENDER, //입찰 선택
	RECV_TENDER_MENU_CHOICE, //입찰메뉴 선택 확인
	SEND_TENDER_CREATE, //입찰물품 생성 선택 
	SEND_TENDER_PARTICIPATE, //입찰 참여 선택
	SEND_BACK, //뒤로가기
	RECV_REQ_TENDER_CREATE,	 //입찰물품 생성 요청	 [프로토콜][char* 물품이름][int 가격][int 입찰자수]
	SEND_TENDER_CREATE_RESULT, //입찰물품 생성 요청 결과
	RECV_REQ_TENDER_LIST, //입찰 리스트 요청
	SEND_TENDER_LIST_RESULT, //입찰 리스트 요청 결과
	RECV_REQ_TENDER_PARTICIPATE, //입찰 요청
	SEND_TENDER_PARTICIPATE_RESULT, //입찰 요청 결과
	SEND_TENDER_END_RESULT //입찰 종료 결과 [프로토콜][char* 메세지]
};

enum RESULT {
	RT_DEFAULT = 0,
	// 로그인매니저 결과
	RT_LOGINMENU = 100, RT_JOINMENU, RT_TENDER, RT_MEMBER_LEAVE, RT_LOGOUT, RT_USER_DISCONNECT,
	RT_ID_OVERLAP, RT_JOIN, RT_EXIT_JOIN, RT_LOGIN, RT_LOGINFAIL,

	RT_CHARACTER_SLOTRESULT = 200,
	RT_CHARACTER_ENTERGAME,				// 게임 접속
	RT_CHARACTER_ENTERGAMEFAIL,			// 게임 접속 실패
	RT_CHARACTER_DELETE,				// 캐릭터 삭제
	RT_CHARACTER_ENTERCREATE, 			// 캐릭터 생성 화면으로 이동
	RT_CHARACTER_CREATE,				// 캐릭터 생성 
	RT_CHARACTER_NICKOVERLAP_TRUE,		// 닉네임 중복
	RT_CHARACTER_EXIT,					// 생성 취소

	RT_INGAME_MOVE = 300,				// 이동
	RT_INGAME_OTHERPLAYER_INFO,			// 다른 플레이어 정보
	RT_INGAME_OTHERPLAYER_LIST,			// 다른 플레이어 리스트
	RT_INGAME_CHANNEL_INFO,				// 채널 정보
	RT_INGAME_CHANNEL_CHANGE,			// 채널 이동
	RT_INGAME_MENU_CHARACTER,			// 캐릭터 선택화면
	RT_INGAME_MENU_LOGOUT,				// 로그아웃
	RT_INGAME_MENU_EXIT					// 종료
};

// 직업코드
enum CHARACTER_JOB
{
	TANKER = 1000,
	WARRIOR = 2000,
	MAGICIAN = 3000,
	GUNNER = 4000
};

enum IOTYPE {
	IOTYPE_ACCEPT = 200, IOTYPE_RECV, IOTYPE_SEND, IOTYPE_ONESIDED_SEND
};

struct WSAOVERLAPPEDEx {
	WSAOVERLAPPED overlapped;
	void* p;
	IOTYPE type;
};

#define USERCOUNT 200
#define CLIENTCOUNT 255
#define SERVERPORT 12345
#define BUFSIZE 512
#define JOBCODE_SIZE 4
#define NICKNAME_CODESIZE 6
#define IDSIZE 30
#define NICKNAMESIZE 20
#define CHARACTERCODESIZE 30
#define PRODUCT_NAMESIZE 50
#define CHARACTER_SPAWNPOS_MAXCOUNT 6
#define ENCRYPT_KEY 951324896

#define MAXCHARACTERORIGIN 4
#define MOVETIME 0.02

#endif
