#ifndef GLOBAL_H
#define GLOBAL_H

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>
#include <math.h>
#include "Vector3.h"

// ��Ŷ : [��üũ�� 4Byte][�������� 4Byte][������]
// ���ڿ� ������ [strlen(���ڿ�)+1 4Byte][���ڿ�]
enum PROTOCOL {
	EXIT = -1,
	CLIENT_LOGOUT_MENU_CHOICE = 0,			// �α׾ƿ� �޴� ����			[��������][int]
	CLIENT_LOGIN_MENU_CHOICE,				// �α��� �޴� ����				[��������][int]
	CLIENT_REQ_EXIT_JOIN,					// ȸ������ ��� ����			[��������]
	CLIENT_REQ_ID_OVERLAP_CHECK,			// ȸ������ ID�ߺ�üũ ��û		[��������][int][char *]
	CLIENT_REQ_JOIN,						// ȸ������ ��û				[��������][int][char *]
	CLIENT_REQ_LOGIN,						// �α��� ��û					[��������][int][char *][int][char *]

	SERVER_JOIN,							// ȸ������ �޴� ����			[��������]
	SERVER_LOGIN,							// �α��� �޴� ����				[��������]
	SERVER_EXIT_JOIN,						// ȸ������ ��� ���			[��������]
	SERVER_ID_OVERLAP_CHECK,				// ID�ߺ� ��� ����				[��������][bool]
	SERVER_JOIN_SUCCESS,					// ȸ������ ��� �޼���			[��������]
	SERVER_LOGIN_SUCCESS,					// �α��� ���� ����				[��������][bool]

	CLIENT_REQ_CHARACTER_SLOT,				// Ŭ�� �ɸ��� ���� ��û		[��������]
	CLIENT_CHARACTER_ENTER,					// Ŭ�� �ɸ��� ����				[��������][int]
	CLIENT_CHARACTER_DELETE,				// Ŭ�� �ɸ��� ����				[��������][int]
	CLIENT_NEW_CHARACTER_MENU,				// Ŭ�� �ɸ��ͻ��� �޴�			[��������]
	CLIENT_REQ_NICK_OVERLAP_CHECK,			// Ŭ�� �г��� �ߺ�				[��������][int][char*]
	CLIENT_REQ_CHARACTER,					// Ŭ�� �ɸ��� ���� ��û		[��������]
	CLIENT_CHARACTER_EXIT,					// Ŭ�� �ɸ��� ���� ���		[��������]
	CLIENT_LOGOUT,							// Ŭ�� �α׾ƿ�				[��������]
	CLIENT_LEAVE,							// Ŭ�� ȸ��Ż��				[��������]

	SERVER_CHARACTER_SLOT_RESULT,			// ���� �ɸ��� ���� ����		[��������]...
	SERVER_CHARACTER_ENTER_RESULT,			// ���� �ɸ��� ���Ӱ��			[��������][bool]
	SERVER_CHARACTER_ENTER_INFO,			// ���� �ɸ��� ���� ����		[DataProtocol��]
	SERVER_CHARACTER_DELETE_RESULT,			// ���� �ɸ��� ���� ���		[��������][bool]
	SERVER_CHARACTER_MENU,					// ���� �ɸ��� �޴� ���		[��������]
	//SERVER_NICK_OVERLAP_CHECK,			// ���� �г��� �ߺ� ���		[��������][bool]
	SERVER_CHARACTER_RESULT,				// ���� �ɸ��� ���� Ȯ�� ���	[��������][bool]
	SERVER_CHARACTER_EXIT_RESULT,			// ���� �ɸ��� ���� ��� ���	[��������[
	SERVER_LEAVE,							// ���� ȸ��Ż�� ���			[��������][bool]
	SERVER_LOGOUT,							// ���� �α׾ƿ� ���			[��������][bool]

	SERVER_TENDER,							// ���� ����
	CLIENT_TENDER_MENU_CHOICE,				// �����޴� ���� Ȯ��
	SERVER_TENDER_CREATE,					// ������ǰ ���� ���� 
	SERVER_TENDER_PARTICIPATE,				// ���� ���� ����
	SERVER_BACK,							// �ڷΰ���
	CLIENT_REQ_TENDER_CREATE,				// ������ǰ ���� ��û	 [��������][char* ��ǰ�̸�][int ����][int �����ڼ�]
	SERVER_TENDER_CREATE_RESULT,			// ������ǰ ���� ��û ���
	CLIENT_REQ_TENDER_LIST,					// ���� ����Ʈ ��û
	SERVER_TENDER_LIST_RESULT,				// ���� ����Ʈ ��û ���
	CLIENT_REQ_TENDER_PARTICIPATE,			// ���� ��û
	SERVER_TENDER_PARTICIPATE_RESULT,		// ���� ��û ���
	SERVER_TENDER_END_RESULT,				// ���� ���� ��� [��������][char* �޼���]

	CLIENT_INGAME_OTHERPLAYERLIST,			// Ŭ�� �ٸ��÷��̾� ����Ʈ ��û	[��������][]
	CLIENT_INGAME_MOVE,						// Ŭ�� �̵�					[��������][���⺤�� or WASD]
	SEVER_INGAME_MOVE_RESULT,				// ���� �̵� ���				[��������][FLOAT][FLOAT][FLOAT][FLOAT : �ð�]
	SEVER_INGAME_OTHERPLAYER_INFO,			// �ٸ��÷��̾� ����			[��������][FLOAT][FLOAT][FLOAT][FLOAT : �ð�][�г��ӻ�����][�г���]
	SEVER_INGAME_OTHERPLAYERLIST_RESULT,		// �÷��̾� ����Ʈ 			[��������][INT:ī��Ʈ][INT:�г��ӻ�����][CHAR:�г���][VECTOR:��ġ]...
	
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
	// �α��θŴ��� ���
	RT_LOGINMENU = 100, RT_JOINMENU, RT_TENDER, RT_MEMBER_LEAVE, RT_LOGOUT, RT_USER_DISCONNECT,
	RT_ID_OVERLAP, RT_JOIN, RT_EXIT_JOIN, RT_LOGIN, RT_LOGINFAIL,

	RT_CHARACTER_SLOTRESULT = 200,
	RT_CHARACTER_ENTERGAME,				// ���� ����
	RT_CHARACTER_DELETE,				// ĳ���� ����
	RT_CHARACTER_ENTERCREATE, 			// ĳ���� ���� ȭ������ �̵�
	RT_CHARACTER_CREATE,				// ĳ���� ���� 
	RT_CHARACTER_NICKOVERLAP_TRUE,		// �г��� �ߺ�
	RT_CHARACTER_EXIT,					// ���� ���

	RT_INGAME_MOVE = 300,				// �̵�
	RT_INGAME_OTHERPLAYER_INFO,			// �ٸ� �÷��̾� ����
	RT_INGAME_OTHERPLAYER_LIST			// �ٸ� �÷��̾� ����Ʈ
};

// �����ڵ�
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
#define IDSIZE 30
#define NICKNAMESIZE 20
#define PRODUCT_NAMESIZE 50
#define CHARACTER_SPAWNPOS_MAXCOUNT 10
#define ENCRYPT_KEY 951324896

#define MAXCHARACTERORIGIN 4
#define MOVETIME 0.02

#endif
