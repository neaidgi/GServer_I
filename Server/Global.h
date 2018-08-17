#ifndef GLOBAL_H
#define GLOBAL_H

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

// ��Ŷ : [��üũ�� 4Byte][�������� 4Byte][������]
// ���ڿ� ������ [strlen(���ڿ�)+1 4Byte][���ڿ�]
enum PROTOCOL {
	EXIT = -1,
	CLIENT_LOGOUT_MENU_CHOICE = 0,			// �α׾ƿ� �޴� ����			[��������][int]
	CLIENT_LOGIN_MENU_CHOICE,				// �α��� �޴� ����				[��������][int]
	SERVER_JOIN,							// ȸ������ �޴� ����			[��������]
	CLIENT_REQ_EXIT_JOIN,					// ȸ������ ��� ����			[��������]
	SERVER_EXIT_JOIN,						// ȸ������ ��� ���			[��������]
	CLIENT_REQ_ID_OVERLAP_CHECK,			// ȸ������ ID�ߺ�üũ ��û		[��������][int][char *]
	SERVER_ID_OVERLAP_CHECK,				// ID�ߺ� ���� ����				[��������][bool]
	CLIENT_REQ_JOIN,						// ȸ������ ��û				[��������][int][char *]
	SERVER_JOIN_SUCCESS,					// ȸ������ ��� �޼���			[��������]
	SERVER_LOGIN,							// �α��� �޴� ����				[��������]
	CLIENT_REQ_LOGIN,						// �α��� ��û					[��������][int][char *][int][char *]
	SERVER_LOGIN_SUCCESS,					// �α��� ���� ����				[��������][bool]
	SERVER_LEAVE,							// ȸ��Ż�� ��� �޼���
	SERVER_LOGOUT,							// �α׾ƿ� ��� �޼���
	
	SERVER_TENDER,							//���� ����
	CLIENT_TENDER_MENU_CHOICE,				//�����޴� ���� Ȯ��
	SERVER_TENDER_CREATE,					//������ǰ ���� ���� 
	SERVER_TENDER_PARTICIPATE,				//���� ���� ����
	SERVER_BACK,							//�ڷΰ���
	CLIENT_REQ_TENDER_CREATE,				//������ǰ ���� ��û	 [��������][char* ��ǰ�̸�][int ����][int �����ڼ�]
	SERVER_TENDER_CREATE_RESULT,			//������ǰ ���� ��û ���
	CLIENT_REQ_TENDER_LIST,					//���� ����Ʈ ��û
	SERVER_TENDER_LIST_RESULT,				//���� ����Ʈ ��û ���
	CLIENT_REQ_TENDER_PARTICIPATE,			//���� ��û
	SERVER_TENDER_PARTICIPATE_RESULT,		//���� ��û ���
	SERVER_TENDER_END_RESULT				//���� ���� ��� [��������][char* �޼���]
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
	RT_ID_OVERLAP, RT_JOIN,RT_EXIT_JOIN, RT_LOGIN, RT_LOGINFAIL,

	// �ٴ��Ŵ��� ���
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
