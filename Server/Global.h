#ifndef GLOBAL_H
#define GLOBAL_H

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

// 패킷 : [전체크기 4Byte][프로토콜 4Byte][데이터]
// 문자열 데이터 [strlen(문자열)+1 4Byte][문자열]
enum PROTOCOL {
	EXIT = -1,
	CLIENT_LOGOUT_MENU_CHOICE = 0,			// 로그아웃 메뉴 선택			[프로토콜][int]
	CLIENT_LOGIN_MENU_CHOICE,				// 로그인 메뉴 선택				[프로토콜][int]
	SERVER_JOIN,							// 회원가입 메뉴 선택			[프로토콜]
	CLIENT_REQ_EXIT_JOIN,					// 회원가입 취소 선택			[프로토콜]
	SERVER_EXIT_JOIN,						// 회원가입 취소 결과			[프로토콜]
	CLIENT_REQ_ID_OVERLAP_CHECK,			// 회원가입 ID중복체크 요청		[프로토콜][int][char *]
	SERVER_ID_OVERLAP_CHECK,				// ID중복 여부 보냄				[프로토콜][bool]
	CLIENT_REQ_JOIN,						// 회원가입 요청				[프로토콜][int][char *]
	SERVER_JOIN_SUCCESS,					// 회원가입 결과 메세지			[프로토콜]
	SERVER_LOGIN,							// 로그인 메뉴 선택				[프로토콜]
	CLIENT_REQ_LOGIN,						// 로그인 요청					[프로토콜][int][char *][int][char *]
	SERVER_LOGIN_SUCCESS,					// 로그인 성공 여부				[프로토콜][bool]
	SERVER_LEAVE,							// 회원탈퇴 결과 메세지
	SERVER_LOGOUT,							// 로그아웃 결과 메세지
	
	SERVER_TENDER,							//입찰 선택
	CLIENT_TENDER_MENU_CHOICE,				//입찰메뉴 선택 확인
	SERVER_TENDER_CREATE,					//입찰물품 생성 선택 
	SERVER_TENDER_PARTICIPATE,				//입찰 참여 선택
	SERVER_BACK,							//뒤로가기
	CLIENT_REQ_TENDER_CREATE,				//입찰물품 생성 요청	 [프로토콜][char* 물품이름][int 가격][int 입찰자수]
	SERVER_TENDER_CREATE_RESULT,			//입찰물품 생성 요청 결과
	CLIENT_REQ_TENDER_LIST,					//입찰 리스트 요청
	SERVER_TENDER_LIST_RESULT,				//입찰 리스트 요청 결과
	CLIENT_REQ_TENDER_PARTICIPATE,			//입찰 요청
	SERVER_TENDER_PARTICIPATE_RESULT,		//입찰 요청 결과
	SERVER_TENDER_END_RESULT				//입찰 종료 결과 [프로토콜][char* 메세지]
};


enum STATE {
	INIT_STATE, LOGIN_STATE,TENDER_STATE,JOIN_STATE,EXIT_STATE,
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
	// 로그인매니저 결과
	RT_LOGINMENU = 100, RT_JOINMENU, RT_TENDER, RT_MEMBER_LEAVE, RT_LOGOUT, RT_USER_DISCONNECT,
	RT_ID_OVERLAP, RT_JOIN,RT_EXIT_JOIN, RT_LOGIN, RT_LOGINFAIL,

	// 텐더매니저 결과
	RT_MENUCREATE, RT_MENUPARTICIPATE, RT_MENUBACK, 
	RT_TENDERCREATE, RT_TENDERPARTICIPATE, RT_TENDERLIST
};

enum IOTYPE {
	IOTYPE_ACCEPT = 200, IOTYPE_RECV, IOTYPE_SEND
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
#define IDSIZE 30
#define NICKNAMESIZE 20
#define PRODUCT_NAMESIZE 50
#define ENCRYPT_KEY 951324896

#endif
