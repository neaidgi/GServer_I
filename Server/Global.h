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
#include "Vector3.h"

// ��Ŷ : [��üũ�� 4Byte][�������� 4Byte][������]
// ���ڿ� ������ [strlen(���ڿ�)+1 4Byte][���ڿ�]
enum PROTOCOL {
	EXIT = -1,
	CLIENT_JOIN_MENU_CHOICE = 0,				// ȸ������ �޴� ����			[��������][int]
	CLIENT_LOGIN_MENU_CHOICE,					// �α��� �޴� ����				[��������][int]
	CLIENT_REQ_EXIT_JOIN,						// ȸ������ ��� ����			[��������]
	CLIENT_REQ_ID_OVERLAP_CHECK,				// ȸ������ ID�ߺ�üũ ��û		[��������][int][char *]
	CLIENT_REQ_JOIN,							// ȸ������ ��û				[��������][int][char *]
	CLIENT_REQ_LOGIN,							// �α��� ��û					[��������][int][char *][int][char *]

	SERVER_JOIN,								// ȸ������ �޴� ����			[��������]
	SERVER_LOGIN,								// �α��� �޴� ����				[��������]
	SERVER_EXIT_JOIN,							// ȸ������ ��� ���			[��������]
	SERVER_ID_OVERLAP_CHECK,					// ID�ߺ� ��� ����				[��������][bool]
	SERVER_JOIN_SUCCESS,						// ȸ������ ��� �޼���			[��������]
	SERVER_LOGIN_SUCCESS,						// �α��� ���� ����				[��������][bool]

	CLIENT_REQ_CHARACTER_SLOT,					// Ŭ�� �ɸ��� ���� ��û		[��������]
	CLIENT_CHARACTER_ENTER,						// Ŭ�� �ɸ��� ����				[��������][int]
	CLIENT_CHARACTER_DELETE,					// Ŭ�� �ɸ��� ����				[��������][int]
	CLIENT_NEW_CHARACTER_MENU,					// Ŭ�� �ɸ��ͻ��� �޴�			[��������]
	CLIENT_REQ_NICK_OVERLAP_CHECK,				// Ŭ�� �г��� �ߺ�				[��������][int][char*]
	CLIENT_REQ_CHARACTER,						// Ŭ�� �ɸ��� ���� ��û		[��������]
	CLIENT_CHARACTER_EXIT,						// Ŭ�� �ɸ��� ���� ���		[��������]
	CLIENT_LOGOUT,								// Ŭ�� �α׾ƿ�				[��������]
	CLIENT_LEAVE,								// Ŭ�� ȸ��Ż��				[��������]

	SERVER_CHARACTER_SLOT_RESULT,				// ���� �ɸ��� ���� ����		[��������]...
	SERVER_CHARACTER_ENTER_RESULT,				// ���� �ɸ��� ���Ӱ��			[��������][bool]
	SERVER_CHARACTER_ENTER_INFO,				// ���� �ɸ��� ���� ����		[DataProtocol��]
	SERVER_CHARACTER_DELETE_RESULT,				// ���� �ɸ��� ���� ���		[��������][bool]
	SERVER_CHARACTER_MENU,						// ���� �ɸ��� �޴� ���		[��������]
	//SERVER_NICK_OVERLAP_CHECK,				// ���� �г��� �ߺ� ���		[��������][bool]
	SERVER_CHARACTER_RESULT,					// ���� �ɸ��� ���� Ȯ�� ���	[��������][bool]
	SERVER_CHARACTER_EXIT_RESULT,				// ���� �ɸ��� ���� ��� ���	[��������[
	SERVER_LEAVE,								// ���� ȸ��Ż�� ���			[��������][bool]
	SERVER_LOGOUT,								// ���� �α׾ƿ� ���			[��������][bool]

	SERVER_TENDER,								// ���� ����
	CLIENT_TENDER_MENU_CHOICE,					// �����޴� ���� Ȯ��
	SERVER_TENDER_CREATE,						// ������ǰ ���� ���� 
	SERVER_TENDER_PARTICIPATE,					// ���� ���� ����
	SERVER_BACK,								// �ڷΰ���
	CLIENT_REQ_TENDER_CREATE,					// ������ǰ ���� ��û	 [��������][char* ��ǰ�̸�][int ����][int �����ڼ�]
	SERVER_TENDER_CREATE_RESULT,				// ������ǰ ���� ��û ���
	CLIENT_REQ_TENDER_LIST,						// ���� ����Ʈ ��û
	SERVER_TENDER_LIST_RESULT,					// ���� ����Ʈ ��û ���
	CLIENT_REQ_TENDER_PARTICIPATE,				// ���� ��û
	SERVER_TENDER_PARTICIPATE_RESULT,			// ���� ��û ���
	SERVER_TENDER_END_RESULT,					// ���� ���� ���		[��������][char* �޼���]

	CLIENT_INGAME_OTHERPLAYERLIST,				// Ŭ�� �ٸ��÷��̾� ����Ʈ ��û	[��������][]
	CLIENT_INGAME_MOVE_START,					// Ŭ�� �̵� ����					[��������]
	CLIENT_INGAME_MOVE_REPORT,					// Ŭ�� �̵��� ����					[��������]
	CLIENT_INGAME_MOVE_END,						// Ŭ�� �̵� ��					[��������]
	CLIENT_INGAME_MOVE_ROTATION,				// Ŭ�� ȸ��
	SERVER_INGAME_MOVE_ORDER,					// ���� �̵� ���				[��������][FLOAT][FLOAT][FLOAT][FLOAT : �ð�]
	SERVER_INGAME_MOVE_RESULT,					// ���� �̵� ���				[��������][FLOAT][FLOAT][FLOAT][FLOAT : �ð�]
	SERVER_INGAME_MOVE_OTHERPLAYERINFO,			// ���� �ٸ��÷��̾� �̵�����	[��������][FLOAT][FLOAT][FLOAT][FLOAT : �ð�][�г��ӻ�����][�г���]
	SERVER_INGAME_MOVE_ROTATION,				// Ŭ�� ȸ��
	SERVER_INGAME_OTHERPLAYERLIST_RESULT,		// ���� �÷��̾� ����Ʈ			[��������][INT:ī��Ʈ][INT:�г��ӻ�����][CHAR:�г���][VECTOR:��ġ]...
	SERVER_INGAME_OTHERPLAYER_CONNECT,			// ���� �÷��̾� ���� ����

	CLIENT_INGAME_MENU_REQ_CHARACTER,			// Ŭ�� ĳ���� ����ȭ������ ��û
	CLIENT_INGAME_MENU_REQ_LOGOUT,				// Ŭ�� �α׾ƿ�
	CLIENT_INGAME_MENU_EXIT,					// Ŭ�� ��������
	SERVER_INGAME_MENU_RESULT_CHARACTER,		// ���� ĳ���� ����ȭ�� ���
	SERVER_INGAME_MENU_RESULT_LOGOUT,			// ���� �α׾ƿ� ���
	SERVER_INGAME_OTHERPLAYER_LEAVE,			// ���� �ٸ� ������ ä�ο��� ��������

	CLIENT_INGAME_CHANNEL_INFO,					// Ŭ�� ä�� ���� ��û
	CLIENT_INGAME_CHANNEL_CHANGE,				// Ŭ�� ä�� �̵� ��û
	SERVER_INGAME_CHANNLE_INFO_RESULT,			// ���� ä�� ���� ���
	SERVER_INGAME_CHANNLE_CHANGE_RESULT,		// ���� ä�� �̵� ��û ���
	SERVER_INGAME_CHANNLE_USER_CHANGE,			// ���� ä�� �̵� ��û�� �� ���� �̵��ߴٰ� �뺸

	CLIENT_INGAME_PARTY_ROOM_JOIN,				// Ŭ�� ��Ƽ ���� ��û
	CLIENT_INGAME_PARTY_ROOM_INVITE,			// Ŭ�� ��Ƽ �ʴ� ��û
	CLIENT_INGAME_PARTY_ROOM_ANSWER_INVITE,		// Ŭ�� ��Ƽ �ʴ� ����
	CLIENT_INGAME_PARTY_USER_KICK,				// Ŭ�� ��Ƽ�� ���� ��û
	CLIENT_INGAME_PARTY_USER_LEAVE,				// Ŭ�� ��Ƽ Ż�� ��û
	CLIENT_INGAME_PARTY_LEADER_DELEGATE,		// Ŭ�� ��Ƽ�� ����

	SERVER_INGAME_PARTY_ROOM_INVITE,			// ���� ��Ƽ �ʴ�(Ư����������)
	SERVER_INGAME_PARTY_ROOM_INVITE_RESULT,		// ���� ��Ƽ �ʴ� ���
	SERVER_INGAME_PARTY_ROOM_JOIN_RESULT,		// ���� ��Ƽ ���� ���
	SERVER_INGAME_PARTY_ROOM_ADD_USER,			// ���� ���ο� ��Ƽ���� ����
	SERVER_INGAME_PARTY_USER_KICK,				// ���� ��Ƽ ������ѻ������ ������ ��������
	SERVER_INGAME_PARTY_USER_KICK_INFO,			// ���� ��Ƽ ������ߴٴ����� �������� 
	SERVER_INGAME_PARTY_USER_KICK_RESULT,		// ���� ��Ƽ ������
	SERVER_INGAME_PARTY_USER_LEAVE_INFO,		// ���� ��Ƽ Ż���ϴ� ���� ����
	SERVER_INGAME_PARTY_USER_LEAVE_RESULT,		// ���� ��Ƽ Ż����
	SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT,		// ���� ��Ƽ�� ���Ľ� �����ִ� �������� ������ ��������
	SERVER_INGAME_PARTY_LEADER_DELEGATE,		// ���� ��Ƽ�� ���� ���� ���� ����
	SERVER_INGAME_PARTY_LEADER_DELEGATE_RESULT,	// ���� ��Ƽ�� ���� ���
	SERVER_INGAME_PARTY_USER_INFO,				// ���� ��Ƽ������(��Ƽ������,�г���,HP,MP)

	// ���� ��������
	CLIENT_INGAME_DUNGEON_ENTER,				// Ŭ�� ���� ����
	CLIENT_INGAME_DUNGEON_LEAVE,				// Ŭ�� ���� ����
	CLIENT_INGAME_DUNGEON_STAGE_IN,				// Ŭ�� ���� ���������� �����ϰڴٴ� ��������(����������)

	SERVER_INGAME_DUNGEON_ENTER_RESULT,			// ���� ���� ���� ���
	SERVER_INGAME_DUNGEON_LEAVE_RESULT,			// ���� ���� ���� ���
	SERVER_INGAME_DUNGEON_STAGE_IN_REULST,		// ���� ���� ���������� ������

	// ���� ��������
	CLIENT_INGAME_MONSTER_INFO,					// Ŭ�� ���� ���� ��û
	CLIENT_INGAME_MONSTER_MOVE,					// Ŭ�� ��Ƽ������ ���� �̵������� ������

	SERVER_INGAME_MONSTER_INFO,					// ���� ���� ������ �ִ� ��������
	SERVER_INGAME_MONSTER_MOVE_RESULT,			// ���� ���� �̵�����

	// ä�� ��������
	CLIENT_REQ_CHAT_LIST,					// ���� �α����� ȸ������Ʈ ��û 			 �������ݸ�
	SERVER_CHAT_LIST_RESULT,				// ���� �α����� ȸ������Ʈ ��û ��� 		[�ο� ��] [�г��� ������] [�г���]...
	CLIENT_REQ_CHAT_START,					// ä�� ��û 								[�ο� ��] [�г��� ������] [�г���]
	SERVER_CHAT_START_RESULT,				// ä�� ��û ��� 							[BOOL��]  ?  [�� ��ȣ] : [���� �ڵ�] [�޼��� ������] [�޼���]
	SERVER_CHAT_MSG,						// ä�� �޼��� : ���� --> Ŭ��				[�� ��ȣ] [�޼��� ������] [�޼���]
	CLIENT_CHAT_MSG,						// ä�� �޼��� : Ŭ�� --> ����				[�� ��ȣ] [�޼��� ������] [�޼���]
	SERVER_CHAT_ENTER,						// ���� ä�ù� ���� : ���� --> Ŭ��			[�� ��ȣ] [�г��� ������] [�г���]
	CLIENT_CHAT_EXIT,						// ���� ä�ù� ���� : Ŭ�� --> ����			[�� ��ȣ] [�г��� ������] [�г���]
	SERVER_CHAT_EXIT						// ä�ù� ���� : ���� --> Ŭ��				[�� ��ȣ]
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

	SEND_TENDER, //���� ����
	RECV_TENDER_MENU_CHOICE, //�����޴� ���� Ȯ��
	SEND_TENDER_CREATE, //������ǰ ���� ���� 
	SEND_TENDER_PARTICIPATE, //���� ���� ����
	SEND_BACK, //�ڷΰ���
	RECV_REQ_TENDER_CREATE,	 //������ǰ ���� ��û	 [��������][char* ��ǰ�̸�][int ����][int �����ڼ�]
	SEND_TENDER_CREATE_RESULT, //������ǰ ���� ��û ���
	RECV_REQ_TENDER_LIST, //���� ����Ʈ ��û
	SEND_TENDER_LIST_RESULT, //���� ����Ʈ ��û ���
	RECV_REQ_TENDER_PARTICIPATE, //���� ��û
	SEND_TENDER_PARTICIPATE_RESULT, //���� ��û ���
	SEND_TENDER_END_RESULT //���� ���� ��� [��������][char* �޼���]
};

enum RESULT {
	RT_DEFAULT = 0,
	// �α��θŴ��� ���
	RT_LOGINMENU = 100, RT_JOINMENU, RT_TENDER, RT_MEMBER_LEAVE, RT_LOGOUT, RT_USER_DISCONNECT,
	RT_ID_OVERLAP, RT_JOIN, RT_EXIT_JOIN, RT_LOGIN, RT_LOGINFAIL,

	RT_CHARACTER_SLOTRESULT = 200,
	RT_CHARACTER_ENTERGAME,					// ���� ����
	RT_CHARACTER_ENTERGAMEFAIL,				// ���� ���� ����
	RT_CHARACTER_DELETE,					// ĳ���� ����
	RT_CHARACTER_ENTERCREATE, 				// ĳ���� ���� ȭ������ �̵�
	RT_CHARACTER_CREATE,					// ĳ���� ���� 
	RT_CHARACTER_NICKOVERLAP_TRUE,			// �г��� �ߺ�
	RT_CHARACTER_EXIT,						// ���� ���

	RT_INGAME_MOVE = 300,					// �̵�
	RT_INGAME_OTHERPLAYER_INFO,				// �ٸ� �÷��̾� ����
	RT_INGAME_OTHERPLAYER_LIST,				// �ٸ� �÷��̾� ����Ʈ
	RT_INGAME_CHANNEL_INFO,					// ä�� ����
	RT_INGAME_CHANNEL_CHANGE,				// ä�� �̵�
	RT_INGAME_MENU_CHARACTER,				// ĳ���� ����ȭ��
	RT_INGAME_MENU_LOGOUT,					// �α׾ƿ�
	RT_INGAME_MENU_EXIT,					// ����
	RT_INGAME_PARTY_INVITE,					// ��Ƽ �ʴ�
	RT_INGAME_PARTY_INVITE_RESULT,			// ��Ƽ �ʴ� ���
	RT_INGAME_PARTY_USER_KICK_RESULT,		// ���� ���
	RT_INGAME_PARTY_USER_LEAVE_RESULT,		// Ż�� ���
	RT_INGAME_PARTY_LEADER_DELEGATE_RESULT,	// ��Ƽ�� ���� ���
	RT_INGAME_DUNGEON_ENTER_RESULT,			// ���� ���� ���
	RT_INGAME_DUNGEON_LEAVE_RESULT,			// ���� ���� ���
	RT_INGAME_DUNGEON_MONSTER_INFO_RESULT,	// ���� ���� ���� ���
	RT_INGAME_DUNGEON_STAGE_IN_RESULT,		// ���� �������� ���� ���
};

// �����ڵ�
enum CHARACTER_JOB
{
	TANKER = 1000,
	WARRIOR = 2000,
	MAGICIAN = 3000,
	GUNNER = 4000
};

// �����ڵ�
enum MONSTER_ORIGINCODE
{
	SPIDER = 1001,
	WORM = 1002,
	BOSS_SPIDER = 1003

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
#define ENCRYPT_KEY 951324896

#define MAXCHARACTERORIGIN 4
// ���� ����
#define MAXMONSTERORIGIN 3
// ù��° �������� ���� ����
#define FIRSTSTAGE_NORMALMONSTER_1 10
#define MOVETIME 0.02

#endif

/*
	[�ʴ��ϴ»�� - Ŭ�� -]
	1.�ʴ��û����(�����ڵ�)																					/ CLIENT_INGAME_PARTY_ROOM_INVITE // Ŭ�� ��Ƽ �ʴ� ��û

	[����]
	2.�ʴ��û����(�����ڵ�)																					/ CLIENT_INGAME_PARTY_ROOM_INVITE // Ŭ�� ��Ƽ �ʴ� ��û
	2.1 �ʴ��û��������� ���� ��Ƽ�������ִ��� Ȯ���ϰ� ��Ƽ�� �������������� ��Ƽ�� �ϳ������ �������� �����Ѵ�.
	3.����ִ���Ȯ���ϰ� ������ �ʴ��û��������������(��Ƽ���ȣ,�����ڵ�,�����г���)							/ SERVER_INGAME_PARTY_ROOM_INVITE // ���� ��Ƽ �ʴ�(Ư����������)
	3-1.�ʴ�޴� ������ ���� �׷��� �ʴ���и� ����(false)														/ SERVER_INGAME_PARTY_ROOM_INVITE_RESULT // ���� ��Ƽ �ʴ� ���
	3-2.�����ϸ� ��Ƽ���� ���ְ� ������ ��Ƽ���¸� �ʱ�ȭ��Ų��.

	[�ʴ��ϴ»�� - Ŭ�� -]
	3-3. �ʴ���й��� -��-																						/ SERVER_INGAME_PARTY_ROOM_INVITE_RESULT // ���� ��Ƽ �ʴ� ���

	[�ʴ�޴»�� - Ŭ�� -]
	4.�ʴ��û�� ����(��Ƽ���ȣ,�����ڵ�,�����г��� )															/ SERVER_INGAME_PARTY_ROOM_INVITE // ���� ��Ƽ �ʴ�(Ư����������)
	5.�ʴ� ������ ����(���ȣ,�����ڵ�,true)																	/ CLIENT_INGAME_PARTY_ROOM_ANSWER_INVITE // Ŭ�� ��Ƽ �ʴ� ���� - true
	5-1.�ʴ� ������ ����(���ȣ,�����ڵ�,false)																	/ CLIENT_INGAME_PARTY_ROOM_ANSWER_INVITE // Ŭ�� ��Ƽ �ʴ� ���� - false

	[����]
	5-2. �ʴ� ������ ����																						/ CLIENT_INGAME_PARTY_ROOM_ANSWER_INVITE // Ŭ�� ��Ƽ �ʴ� ����
	5-3. �ʴ� ����ģ������ ����������(false) - ���ȣ�� ���ؼ� ���忡�Ժ�����									/ SERVER_INGAME_PARTY_ROOM_INVITE_RESULT // ���� ��Ƽ �ʴ� ���

	[�ʴ��ϴ»�� - Ŭ�� - ]
	5-4. �ʴ� ������ ���� -��-																					/ SERVER_INGAME_PARTY_ROOM_INVITE_RESULT // ���� ��Ƽ �ʴ� ���

	[����]
	6.�ʴ� ��������(���ȣ,�����ڵ�,true)																		/ CLIENT_INGAME_PARTY_ROOM_ANSWER_INVITE // Ŭ�� ��Ƽ �ʴ� ����
	7.�濡 ���� �ְ� �����ϸ� ���� ������ �������Ѵ�.
	7-1. ��Ƽ�濡 �����ִ°� �����Ѵ�. �׷��� �����ߴٰ� ��Ŷ�� ������(false)									/ SERVER_INGAME_PARTY_ROOM_JOIN_RESULT // ���� ��Ƽ ���� ���

	[�ʴ�޴»�� - Ŭ�� - ]
	7-2. ��Ƽ���� ���и� �޴´�. - ��-																			/ SERVER_INGAME_PARTY_ROOM_JOIN_RESULT // ���� ��Ƽ ���� ���

	[����]
	8.���ȣ ����鿡�� �ʴ���� ������ ������ ����(�����ڵ�,�����г���,��������,����ü��,��������)				/ SERVER_INGAME_PARTY_ROOM_ADD_USER // ���� ���ο� ��Ƽ���� ����
	9.�ʴ�޴»������ ��Ƽ�������� ������ ������(��������,�����ڵ�,�����г���,��������,����ü��,��������)		/ SERVER_INGAME_PARTY_ROOM_JOIN_RESULT // ���� ��Ƽ ���� ���

	[�ʴ��ϴ»�� - Ŭ�� - ]
	8-1. �������� ����(�����ڵ�,�����г���,��������,����ü��,��������) -��-										/ SERVER_INGAME_PARTY_ROOM_ADD_USER // ���� ���ο� ��Ƽ���� ����

	[��Ƽ�� - Ŭ�� - ]
	8-2.������������(�����ڵ�,�����г���,��������,����ü��,��������) -��-										/ SERVER_INGAME_PARTY_ROOM_ADD_USER // ���� ���ο� ��Ƽ���� ����

	[�ʴ�޴»�� - Ŭ�� - ]
	9-1.���� ������ �޴´�(��������,�����ڵ�,�����г���,��������,����ü��,��������) -��-						/ SERVER_INGAME_PARTY_ROOM_JOIN_RESULT // ���� ��Ƽ ���� ���


*/