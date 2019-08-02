#ifndef GLOBAL_H
#define GLOBAL_H

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>
#include <math.h>
#include <queue>
#include <list>
#include <vector>
#include <ctime>
#include <complex.h>
#include "Vector3.h"

// 패킷 : [전체크기 4Byte][프로토콜 4Byte][데이터]
// 문자열 데이터 [strlen(문자열)+1 4Byte][문자열]

// AND연산용 변수
#define PROTOCOL_FFFFFFFFFFFFFFFF 0xFFFFFFFFFF000000

// -- 서버 -- //

// 큰틀
#define PROTOCOL_80 0x80										// [큰틀 - 미정]
#define PROTOCOL_40 0x40										// [큰틀 - 미정]
#define PROTOCOL_20 0x20										// [큰틀 - 미정]
#define PROTOCOL_10 0x10										// [큰틀 - 미정]

#define PROTOCOL_8 0x8											// [큰틀 - 미정]
#define PROTOCOL_INGAME 0x4										// [큰틀 - 인게임]
#define PROTOCOL_CHRACTER 0x2									// [큰틀 - 캐릭터선택화면]
#define PROTOCOL_TITLE 0x1										// [큰틀 - 타이틀]

// 타이틀 중간틀
#define PROTOCOL_LOGIN 0x100									// [중간틀 - 타이틀]

// 타이틀 세부틀(PROTOCOL_LOGIN)
#define PROTOCOL_SERVER_LOGIN_MENU_COMPART 0x7000000			// [세부틀 - 타이틀] [프로토콜 연산용]

#define PROTOCOL_LOGIN_RESULT 0x4000000							// [세부틀 - 타이틀] [서버->클라] 로그인 결과
#define PROTOCOL_JOIN_RESULT 0x2000000							// [세부틀 - 타이틀] [서버->클라] 회원가입 결과
#define PROTOCOL_ID_OVERLAP_CHECK 0x1000000						// [세부틀 - 타이틀] [서버->클라] ID 중복체크 결과

// 캐릭터 중간틀
#define PROTOCOL_CHARACER_MENU 0x100							// [중간틀 - 캐릭터선택화면]

// 캐릭터 세부틀(PROTOCOL_CHARACER_MENU)
#define PROTOCOL_SERVER_CHARACTER_MENU_COMPART 0xF000000		// [세부틀 - 캐릭터선택화면] [프로토콜 연산용]

#define PROTOCOL_CHARACTER_SLOT_INFO 0x8000000					// [세부틀 - 캐릭터선택화면] [서버->클라] 캐릭터 슬롯 정보		[프로토콜]...
#define PROTOCOL_CHARACTER_ENTER_RESULT	0x4000000				// [세부틀 - 캐릭터선택화면] [서버->클라] 캐릭터 접속결과		[프로토콜][bool]
#define PROTOCOL_CHARACTER_DELETE_RESULT 0x2000000				// [세부틀 - 캐릭터선택화면] [서버->클라] 캐릭터 삭제 결과		[프로토콜][bool]
#define PROTOCOL_CHARACTER_RESULT 0x1000000						// [세부틀 - 캐릭터선택화면] [서버->클라] 캐릭터 생성 확인 결과	[프로토콜][bool]

// 인게임 중간틀				
#define PROTOCOL_INGMAE_MENU 0x4000								// [중간틀 - 인게임] 매뉴관련
#define PROTOCOL_INGAME_ANIMATION 0x2000						// [중간틀 - 인게임] 애니메이션 관련
#define PROTOCOL_INGAME_DUNGEON 0x1000							// [중간틀 - 인게임] 던전관련

#define PROTOCOL_INGAME_PARTY 0x800								// [중간틀 - 인게임] 파티관련
#define PROTOCOL_INGAME_CHANNEL 0x400							// [중간틀 - 인게임] 채널관련
#define PROTOCOL_INGMAE_MONSTER 0x200							// [중간틀 - 인게임] 몬스터관련
#define PROTOCOL_INGAME_CHARACER 0x100							// [중간틀 - 인게임] 캐릭터관련

// 인게임 세부틀(PROTOCOL_INGAME_CHARACER) - 캐릭터 관련
#define PROTOCOL_SERVER_INGAME_CHARACTER_COMPART 0x7FF0000000	// [세부틀 - 캐릭터관련] [프로토콜 연산용]

#define PROTOCOL_INGAME_OTHERPLAYER_ATTACK_SUCCESS 0x4000000000	// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 공격성공했다
#define PROTOCOL_INGAME_ATTACK_RESULT 0x2000000000				// [세부틀 - 캐릭터관련] [서버->클라] 유저 공격 결과
#define PROTOCOL_INGAME_OTHERPLAYER_ATTACK 0x1000000000			// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 공격했다

#define PROTOCOL_INGAME_MOVE_OTHERPLAYER_END_JUMP 0x800000000	// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 점프 끝났다는 정보(착지)
#define PROTOCOL_INGAME_MOVE_OTHERPLAYER_START_JUMP 0x400000000	// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 점프 했다는 정보
#define PROTOCOL_INGAME_OTHERPLAYER_LEAVE 0x200000000			// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 나간정보
#define PROTOCOL_INGAME_OTHERPLAYER_CONNECT 0x100000000			// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 접속정보

#define PROTOCOL_INGAME_MOVE_ROTATION 0x80000000				// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 회전 정보
#define PROTOCOL_INGAME_MOVE_OTHERPLAYERINFO 0x40000000			// [세부틀 - 캐릭터관련] [서버->클라] 다른 유저 이동 정보
#define PROTOCOL_INGAME_MOVE_RESULT 0x20000000					// [세부틀 - 캐릭터관련] [서버->클라] 유저 이동 결과
#define PROTOCOL_INGAME_OTHERPLAYERLIST_INFO 0x10000000			// [세부틀 - 캐릭터관련] [서버->클라] 유저들 정보

// 인게임 세부틀(PROTOCOL_INGMAE_MONSTER) - 몬스터 관련
#define PROTOCOL_SERVER_INGAME_MONSTER_COMPART 0xF0000000		// [세부틀 - 던전관련] [프로토콜 연산용]

#define PROTOCOL_INGAME_MONSTER_OTHER_ATTACK_RESULT 0x80000000	// [세부틀 - 몬스터관련] [서버->클라] 몬스터 다른유저 공격결과
#define PROTOCOL_INGAME_MONSTER_ATTACK_RESULT 0x40000000		// [세부틀 - 몬스터관련] [서버->클라] 몬스터 유저 공격결과
#define PROTOCOL_MONSTER_INFO 0x20000000						// [세부틀 - 몬스터관련] [서버->클라] 정보를 주는 프로토콜
#define PROTOCOL_MONSTER_MOVE_RESULT 0x10000000					// [세부틀 - 몬스터관련] [서버->클라] 몬스터 이동정보

// 인게임 세부틀(PROTOCOL_INGAME_CHANNEL) - 채널 관련
#define PROTOCOL_SERVER_INGAME_CHANNEL_COMPART 0x7000000		// [세부틀 - 채널관련] [프로토콜 연산용]

#define PROTOCOL_CHANNLE_USER_CHANGE 0x4000000					// [세부틀 - 채널관련] [서버->클라] 채널 이동한 유저 정보
#define PROTOCOL_CHANNLE_CHANGE_RESULT 0x2000000				// [세부틀 - 채널관련] [서버->클라] 채널 변경 결과
#define PROTOCOL_CHANNLE_INFO 0x1000000							// [세부틀 - 채널관련] [서버->클라] 채널 정보

// 인게임 세부틀(PROTOCOL_INGAME_PARTY) - 파티 관련
#define PROTOCOL_SERVER_INGAME_PARTY_COMPART 0x1FFF0000000		// [세부틀 - 파티관련] [프로토콜 연산용]

#define PROTOCOL_PARTY_USER_INFO 0x10000000000					// [세부틀 - 파티관련] [서버->클라] 파티원정보

#define PROTOCOL_PARTY_LEADER_DELEGATE_RESULT 0x8000000000		// [세부틀 - 파티관련] [서버->클라] 리더 위임 결과
#define PROTOCOL_PARTY_LEADER_DELEGATE 0x4000000000				// [세부틀 - 파티관련] [서버->클라] 리더 위임 받은 유저 정보
#define PROTOCOL_PARTY_ROOM_REMOVE_RESULT 0x2000000000			// [세부틀 - 파티관련] [서버->클라] 파티방 사라졌을때 보내는 프로토콜
#define PROTOCOL_PARTY_USER_LEAVE_RESULT 0x1000000000			// [세부틀 - 파티관련] [서버->클라] 탈퇴결과

#define PROTOCOL_PARTY_USER_LEAVE_INFO 0x800000000				// [세부틀 - 파티관련] [서버->클라] 탈퇴하는 유저 정보
#define PROTOCOL_PARTY_USER_KICK_RESULT 0x400000000				// [세부틀 - 파티관련] [서버->클라] 강퇴결과
#define PROTOCOL_PARTY_USER_KICK_INFO 0x200000000				// [세부틀 - 파티관련] [서버->클라] 강퇴당한 유저 정보
#define PROTOCOL_PARTY_USER_KICK 0x100000000					// [세부틀 - 파티관련] [서버->클라] 강퇴당한사람에게 보내는 프로토콜

#define PROTOCOL_PARTY_ROOM_ADD_USER 0x80000000					// [세부틀 - 파티관련] [서버->클라] 새로운 파티원이 들어옴
#define PROTOCOL_PARTY_ROOM_JOIN_RESULT 0x40000000				// [세부틀 - 파티관련] [서버->클라] 참여결과
#define PROTOCOL_PARTY_ROOM_INVITE_RESULT 0x20000000			// [세부틀 - 파티관련] [서버->클라] 파티 초대 결과
#define PROTOCOL_PARTY_ROOM_INVITE 0x10000000					// [세부틀 - 파티관련] [서버->클라] 파티 초대(특정유저에게)

// 인게임 세부틀(PROTOCOL_INGAME_DUNGEON) - 던전 관련
#define PROTOCOL_SERVER_INGAME_DUNGEON_COMPART 0x70000000		// [세부틀 - 던전관련] [프로토콜 연산용]

#define PROTOCOL_DUNGEON_STAGE_IN_RESULT 0x40000000				// [세부틀 - 던전관련] [서버->클라] 스테이지에 입장결과
#define PROTOCOL_DUNGEON_LEAVE_RESULT 0x20000000				// [세부틀 - 던전관련] [서버->클라] 던전 퇴장 결과
#define PROTOCOL_DUNGEON_ENTER_RESULT 0x10000000				// [세부틀 - 던전관련] [서버->클라] 던전 입장 결과

// 인게임 세부틀(PROTOCOL_INGAME_ANIMATION) - 애니메이션 관련
#define PROTOCOL_SERVER_INGAME_ANIMATION_COMPART 0x7000000		// [세부틀 - 애니메이션관련] [프로토콜 연산용]

#define PROTOCOL_MONSTER_ANIMATION_ATTACK 0x4000000				// [세부틀 - 애니메이션관련] [서버->클라] 몬스터 애니메이션(공격)
#define PROTOCOL_MONSTER_ANIMATION 0x2000000					// [세부틀 - 애니메이션관련] [서버->클라] 몬스터 애니메이션
#define PROTOCOL_CHARACER_ANIMATION 0x1000000					// [세부틀 - 애니메이션관련] [서버->클라] 캐릭터 애니메이션

// 인게임 세부틀(PROTOCOL_INGMAE_MENU) - 매뉴 관련
#define PROTOCOL_SERVER_INGAME_MENU_COMPART 0x7000000				// [세부틀 - 몬스터관련] [프로토콜 연산용]

#define PROTOCOL_OTHERPLAYER_LEAVE 0x4000000					// [세부틀 - 몬스터관련] [서버->클라] 다른 유저가 채널에서 나갔을때
#define PROTOCOL_MENU_RESULT_LOGOUT 0x2000000					// [세부틀 - 몬스터관련] [서버->클라] 로그아웃 결과
#define PROTOCOL_MENU_RESULT_CHARACTER 0x1000000				// [세부틀 - 몬스터관련] [서버->클라] 캐릭터 선택화면 결과


// -- 서버 END -- //


// -- 클라 -- //

// 큰틀
#define PROTOCOL_80 0x80										// [큰틀 - 미정]
#define PROTOCOL_40 0x40										// [큰틀 - 미정]
#define PROTOCOL_20 0x20										// [큰틀 - 미정]
#define PROTOCOL_10 0x10										// [큰틀 - 미정]

#define PROTOCOL_8 0x8											// [큰틀 - 미정]
#define PROTOCOL_INGAME 0x4										// [큰틀 - 인게임]
#define PROTOCOL_CHRACTER 0x2									// [큰틀 - 캐릭터선택화면]
#define PROTOCOL_TITLE 0x1										// [큰틀 - 타이틀]

// 타이틀 중간틀
#define PROTOCOL_LOGIN 0x100									// [중간틀 - 타이틀]

// 타이틀 세부틀(PROTOCOL_LOGIN)
#define PROTOCOL_CLIENT_LOGIN_MENU_COMPART 0x7000000			// [세부틀 - 타이틀] [프로토콜 연산용]

#define PROTOCOL_REQ_LOGIN 0x4000000							// [세부틀 - 타이틀] [클라->서버] 로그인 요청
#define PROTOCOL_REQ_JOIN 0x2000000								// [세부틀 - 타이틀] [클라->서버] 회원가입 요청
#define PROTOCOL_REQ_ID_OVERLAP_CHECK 0x1000000					// [세부틀 - 타이틀] [클라->서버] ID 중복체크 요청

// 캐릭터 중간틀
#define PROTOCOL_CHARACER_MENU 0x100							// [중간틀 - 캐릭터선택화면]

// 캐릭터 세부틀(PROTOCOL_CHARACER_MENU)
#define PROTOCOL_CLIENT_CHARACTER_MENU_COMPART 0xF000000		// [세부틀 - 캐릭터선택화면] [프로토콜 연산용]

#define PROTOCOL_REQ_CHARACTER_CREATE 0x8000000					// [세부틀 - 캐릭터선택화면] [클라->서버] 캐릭터 생성 요청
#define PROTOCOL_CHARACTER_DELETE 0x4000000					    // [세부틀 - 캐릭터선택화면] [클라->서버] 캐릭터 삭제 요청
#define PROTOCOL_CHARACTER_ENTER 0x2000000					    // [세부틀 - 캐릭터선택화면] [클라->서버] 캐릭터 접속 요청
#define PROTOCOL_REQ_CHARACTER_SLOT 0x1000000				    // [세부틀 - 캐릭터선택화면] [클라->서버] 캐릭터 슬롯 정보 요총


// 인게임 중간틀				
#define PROTOCOL_INGMAE_MENU 0x4000								// [중간틀 - 인게임] 매뉴관련
#define PROTOCOL_INGAME_ANIMATION 0x2000						// [중간틀 - 인게임] 애니메이션 관련
#define PROTOCOL_INGAME_DUNGEON 0x1000							// [중간틀 - 인게임] 던전관련

#define PROTOCOL_INGAME_PARTY 0x800								// [중간틀 - 인게임] 파티관련
#define PROTOCOL_INGAME_CHANNEL 0x400							// [중간틀 - 인게임] 채널관련
#define PROTOCOL_INGMAE_MONSTER 0x200							// [중간틀 - 인게임] 몬스터관련
#define PROTOCOL_INGAME_CHARACER 0x100							// [중간틀 - 인게임] 캐릭터관련


// 인게임 세부틀(PROTOCOL_INGAME_CHARACER) - 캐릭터 관련
#define PROTOCOL_CLIENT_INGAME_CHARACTER_COMPART 0x7F0000000	// [세부틀 - 캐릭터관련] [프로토콜 연산용]

#define PROTOCOL_INGAME_ATTACK_SUCCESS 0x400000000				// [세부틀 - 캐릭터관련] [클라->서버] 공격했는데 몬스터랑 피격판정이난것같다.
#define PROTOCOL_INGAME_ATTACK 0x200000000						// [세부틀 - 캐릭터관련] [클라->서버] 공격했다는 정보
#define PROTOCOL_INGAME_MOVE_END_JUMP 0x100000000				// [세부틀 - 캐릭터관련] [클라->서버] 점프 끝났다는 정보(착지)

#define PROTOCOL_INGAME_MOVE_START_JUMP 0x80000000				// [세부틀 - 캐릭터관련] [클라->서버] 점프 시작했다는 정보
#define PROTOCOL_MOVE_ROTATION 0x40000000						// [세부틀 - 캐릭터관련] [클라->서버] 회전 정보
#define PROTOCOL_MOVE_REPORT 0x20000000							// [세부틀 - 캐릭터관련] [클라->서버] 이동 정보
#define PROTOCOL_REQ_OTHERPLAYERLIST 0x10000000					// [세부틀 - 캐릭터관련] [클라->서버] 다른 유저 요청

// 인게임 세부틀(PROTOCOL_INGMAE_MONSTER) - 몬스터 관련
#define PROTOCOL_CLIENT_INGAME_MONSTER_COMPART 0x70000000		// [세부틀 - 몬스터관련] [프로토콜 연산용]

#define PROTOCOL_REQ_MONSTER_ATTACK 0x40000000					// [세부틀 - 몬스터관련] [클라->서버] 몬스터 유저를 공격했다
#define PROTOCOL_MONSTER_MOVE 0x20000000						// [세부틀 - 몬스터관련] [클라->서버] 몬스터 이동
#define PROTOCOL_REQ_MONSTER_INFO 0x10000000					// [세부틀 - 몬스터관련] [클라->서버] 몬스터 정보 요청

// 인게임 세부틀(PROTOCOL_INGAME_CHANNEL) - 채널 관련
#define PROTOCOL_CLIENT_INGAME_CHANNEL_COMPART 0x3000000		// [세부틀 - 채널관련] [프로토콜 연산용]

#define PROTOCOL_REQ_CHANNEL_CHANGE 0x2000000					// [세부틀 - 채널관련] [클라->서버] 채널 이동 요청
#define PROTOCOL_REQ_CHANNEL_INFO 0x1000000						// [세부틀 - 채널관련] [클라->서버] 채널 정보 요청

// 인게임 세부틀(PROTOCOL_INGAME_PARTY) - 파티 관련
#define PROTOCOL_CLIENT_INGAME_PARTY_COMPART 0x1F0000000		// [세부틀 - 파티관련] [프로토콜 연산용]

#define PROTOCOL_REQ_LEADER_DELEGATE 0x100000000				// [세부틀 - 파티관련] [클라->서버] 파티장 위임 요청

#define PROTOCOL_REQ_USER_LEAVE 0x80000000						// [세부틀 - 파티관련] [클라->서버] 파티 탈퇴 요청
#define PROTOCOL_REQ_USER_KICK 0x40000000						// [세부틀 - 파티관련] [클라->서버] 파티원 강퇴 요청
#define PROTOCOL_PARTY_ROOM_ANSWER_INVITE 0x20000000			// [세부틀 - 파티관련] [클라->서버] 파티 초대 응답
#define PROTOCOL_REQ_PARTY_ROOM_INVITE 0x10000000				// [세부틀 - 파티관련] [클라->서버] 파티 초대 요청

// 인게임 세부틀(PROTOCOL_INGAME_DUNGEON) - 던전 관련
#define PROTOCOL_CLIENT_INGAME_DUNGEON_COMPART 0x70000000		// [세부틀 - 던전관련] [프로토콜 연산용]

#define PROTOCOL_DUNGEON_STAGE_IN 0x40000000					// [세부틀 - 던전관련] [클라->서버] 스테이지 입장 요청
#define PROTOCOL_REQ_DUNGEON_LEAVE 0x20000000					// [세부틀 - 던전관련] [클라->서버] 던전 나가기 요청
#define PROTOCOL_REQ_DUNGEON_ENTER 0x10000000					// [세부틀 - 던전관련] [클라->서버] 던전 입장 요청

// 인게임 세부틀(PROTOCOL_INGAME_ANIMATION) - 애니메이션 관련
#define PROTOCOL_CLIENT_INGAME_ANIMATION_COMPART 0x7000000		// [세부틀 - 애니메이션관련] [프로토콜 연산용]

#define PROTOCOL_CLIENT_MONSTER_ANIMATION_ATTACK 0x4000000		// [세부틀 - 애니메이션관련] [클라->서버] 몬스터 애니메이션(공격)
#define PROTOCOL_CLIENT_MONSTER_ANIMATION 0x2000000			    // [세부틀 - 애니메이션관련] [클라->서버] 몬스터 애니메이션
#define PROTOCOL_CLIENT_CHARACER_ANIMATION 0x1000000		    // [세부틀 - 애니메이션관련] [클라->서버] 캐릭터 애니메이션

// 인게임 세부틀(PROTOCOL_INGMAE_MENU) - 매뉴 관련
#define PROTOCOL_CLIENT_INGAME_MENU_COMPART 0x7000000		// [세부틀 - 매뉴관련] [프로토콜 연산용]

#define PROTOCOL_MENU_EXIT 0x4000000						    // [세부틀 - 매뉴관련] [클라->서버] 종료
#define PROTOCOL_MENU_REQ_LOGOUT 0x2000000					    // [세부틀 - 매뉴관련] [클라->서버] 로그아웃 요청
#define PROTOCOL_MENU_REQ_CHARACTER 0x1000000				    // [세부틀 - 매뉴관련] [클라->서버] 캐릭터 선택화면으로 요청

// -- 클라 END -- //

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
	RT_CHARACTER_ENTERGAME,					// 게임 접속
	RT_CHARACTER_ENTERGAMEFAIL,				// 게임 접속 실패
	RT_CHARACTER_DELETE,					// 캐릭터 삭제
	RT_CHARACTER_ENTERCREATE, 				// 캐릭터 생성 화면으로 이동
	RT_CHARACTER_CREATE,					// 캐릭터 생성 
	RT_CHARACTER_NICKOVERLAP_TRUE,			// 닉네임 중복
	RT_CHARACTER_EXIT,						// 생성 취소

	RT_INGAME_MOVE = 300,					// 이동
	RT_INGAME_OTHERPLAYER_INFO,				// 다른 플레이어 정보
	RT_INGAME_OTHERPLAYER_LIST,				// 다른 플레이어 리스트
	RT_INGAME_ATTACK,						// 공격
	RT_INGAME_ATTACK_RESULT,				// 공격성공
	RT_INGAME_CHANNEL_INFO,					// 채널 정보
	RT_INGAME_CHANNEL_CHANGE,				// 채널 이동
	RT_INGAME_MENU_CHARACTER,				// 캐릭터 선택화면
	RT_INGAME_MENU_LOGOUT,					// 로그아웃
	RT_INGAME_MENU_EXIT,					// 종료
	RT_INGAME_PARTY_INVITE,					// 파티 초대
	RT_INGAME_PARTY_INVITE_RESULT,			// 파티 초대 결과
	RT_INGAME_PARTY_USER_KICK_RESULT,		// 강퇴 결과
	RT_INGAME_PARTY_USER_LEAVE_RESULT,		// 탈퇴 결과
	RT_INGAME_PARTY_LEADER_DELEGATE_RESULT,	// 파티장 위임 결과
	RT_INGAME_DUNGEON_ENTER_RESULT,			// 던전 입장 결과
	RT_INGAME_DUNGEON_LEAVE_RESULT,			// 던전 퇴장 결과
	RT_INGAME_DUNGEON_MONSTER_INFO_RESULT,	// 던전 몬스터 정보 결과
	RT_INGAME_DUNGEON_STAGE_IN_RESULT,		// 던전 스테이지 입장 결과
	RT_INGMAE_MONSTER_MOVE_RESULT,			// 몬스터 이동 결과
	RT_INGAME_MONSTER_ATTACK				// 몬스터 공격
};

// 직업코드
enum CHARACTER_JOB
{
	TANKER = 1000,
	WARRIOR = 2000,
	MAGICIAN = 3000,
	GUNNER = 4000
};

// 몬스터코드
enum MONSTER_ORIGINCODE
{
	SPIDER = 10001,
	WORM = 10002,
	BOSS_SPIDER = 10003,
	DOG = 20001,
	ORCCANNONSOLDIER = 20002,
	KING_OF_THEAXE = 20003,
	BEAR = 30001,
	DINOSAUR = 30002,
	DRAGON = 30003
};
// 몬스터 공격 코드
enum MONSTER_ATTACK_CODE
{
	SPIDER_FIRST_ATTACK = 10011,
	SPIDER_SECOND_ATTACK = 10021,
	WORM_FIRST_ATTACK = 10012,
	BOSS_SPIDER_FIRST_ATTACK = 10013,
	DOG_FIRST_ATTACK = 20011,
	ORCCANNONSOLDIER_FIRST_ATTACK = 20012,
	BEAR_FIRST_ATTACK = 30011,
	DINOSAUR_FIRST_ATTACK = 30012
};

enum IOTYPE {
	IOTYPE_ACCEPT = 200, IOTYPE_RECV, IOTYPE_SEND, IOTYPE_ONESIDED_SEND
};

enum DUNGEON_STAGE
{
	DEFAULT_STAGE = 0,
	DUNGEON_STAGE_NORMAL_1 = 10,
	DUNGEON_STAGE_BOSS_1,
	DUNGEON_STAGE_NORMAL_2 = 20,
	DUNGEON_STAGE_BOSS_2,
	DUNGEON_STAGE_NORMAL_3 = 30,
	DUNGEON_STAGE_BOSS_3,
	DUNGEON_STAGE_BOSS_4 = 40
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
#define DUNGEON_SPAWNPOS_MAXCOUNT 4
#define MONSTER_SPAWNPOS_MAXCOUNT 3
#define NORMAL_MONSTER_SPAWNPOS_COUNT 2
#define BOSS_MONSTER_SPWANPOS_NUM 2
#define ENCRYPT_KEY 951324896

#define MAXCHARACTERORIGIN 4

// 몬스터 종류숫자
#define MAXMONSTERORIGIN 8
// 몬스터 스폰 시간
#define MONSTERSPAWNTIME 1.5
// 테스트 스테이지 몬스터 숫자
#define TESTSTAGE_MONSTER 1
// 첫번째 스테이지 몬스터 숫자
#define FIRSTSTAGE_NORMALMONSTER_1 1
// 스테이지 숫자
#define STAGE_COUNT 4
// 스테이지 일반몹 숫자
#define STAGE_MONSTER_NUM 2
#define MOVETIME 0.02

// 몬스터 시간
#define MONSTERTIME 0.5

// 360도
#define CIRCLE_THETA 360
//PI정의
#define PI 3.14159265359
// 라디안으로
#define DEG2RAD PI/180
// 디그리로
#define RAD2DEG 180/PI
// 오차범위
#define MARGIN_OF_ERROR 0.05


#endif