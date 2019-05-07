#include "LoginManager.h"
#include "LogManager.h"
#include "DBManager.h"
#include "MsgManager.h"

LoginManager* LoginManager::Instance = nullptr;

LoginManager::LoginManager()
{
	userCount = 0;
}

LoginManager::~LoginManager()
{

}

const UserInfo* LoginManager::getUser(const char* id)

{
	for (int i = 0; i < userCount; i++)
	{
		if (!strcmp(userList[i]->id, id))
		{
			return userList[i];
		}
	}
	return nullptr;
}

void LoginManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new LoginManager();
	}
}

LoginManager* LoginManager::GetInstance()
{
	return Instance;
}

void LoginManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool LoginManager::InitializeManager()
{
	return true;
}

void LoginManager::EndManager()
{
}

// ID �ߺ�üũ
void LoginManager::reqIdOverlapCheck(User* user, char* _buf)
{
	INT64 protocol = 0;
	int len = 0;
	char id[IDSIZE];
	char msg[BUFSIZE];
	bool check = false;

	memset(id, 0, sizeof(IDSIZE));
	memset(msg, 0, sizeof(BUFSIZE));

	// ���̵� ����
	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	// ���̵�
	memcpy(id, _buf, len);


	check = DBManager::GetInstance()->Login_CheckID(id);

	if (!check)
	{
		user->setID(id);
	}
	sprintf(msg, "%s : %s", id, check ? "�ߺ�" : "�ߺ��ƴ�");
	MsgManager::GetInstance()->DisplayMsg("�α���", msg);

	protocol = user->BitPackProtocol(protocol, PROTOCOL_ID_OVERLAP_CHECK);
	user->Quepack(protocol, &check, sizeof(bool));
}

// ȸ������ ���
void LoginManager::reqJoin(User* user, char* _buf)
{
	if (userCount >= USERCOUNT)
		return;

	INT64 protocol = 0;
	int len = 0;
	char id[IDSIZE];
	char pw[IDSIZE];
	char tempbuf[IDSIZE + 10];
	bool result = false;
	bool resultslot = false;

	memset(id, 0, sizeof(IDSIZE));
	memset(pw, 0, sizeof(IDSIZE));
	memset(tempbuf, 0, sizeof(IDSIZE + 10));

	// ���̵� ����
	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	// ���̵�
	memcpy(id, _buf, len);
	_buf += len;

	// ��й�ȣ ����
	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	// ��й�ȣ
	memcpy(pw, _buf, len);

	// �߰����� : ������ �Է��ߴ� �ߺ��� ���̵� �ƴ϶�� ȸ������ ���
	if (strcmp(id, user->getID()))
	{
		result = false;
		protocol = user->BitPackProtocol(protocol, PROTOCOL_JOIN_RESULT);
		user->Quepack(protocol, &result, sizeof(bool));
		return;
	}

	UserInfo* info = new UserInfo();
	strcpy(info->id, user->getID());
	strcpy(info->pw, pw);

	result = DBManager::GetInstance()->Login_Req_Join(info->id, info->pw);

	if (result)
	{
		user->setPW(info->pw);
	}


	protocol = user->BitPackProtocol(protocol, PROTOCOL_JOIN_RESULT);
	user->Quepack(protocol, &result, sizeof(bool));

	sprintf(tempbuf, "%s [ȸ������].", user->getID());
	LogManager::GetInstance()->LogWrite(tempbuf);
	MsgManager::GetInstance()->DisplayMsg("�α���", tempbuf);
}

// �α��� ���
bool LoginManager::reqLogin(User* user, char* _buf)
{
	INT64 protocol = 0;
	int len = 0;
	char id[IDSIZE];
	char pw[IDSIZE];
	char tempbuf[IDSIZE + 10];

	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	memcpy(id, _buf, len);
	_buf += len;

	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	memcpy(pw, _buf, len);

	bool result;

	result = DBManager::GetInstance()->Login_Req_Login(id, pw);

	if (result)
	{
		user->setID(id);
		user->setPW(pw);
		user->setLogin();
		sprintf(tempbuf, "%s [�α���].", user->getID());
	}
	else
	{
		sprintf(tempbuf, "[�α��� ����]");
	}

	MsgManager::GetInstance()->DisplayMsg("�α���", tempbuf);
	protocol = user->BitPackProtocol(protocol, PROTOCOL_LOGIN_RESULT);
	user->Quepack(protocol, &result, sizeof(bool));
	
	return result;
}

// ȸ������ ȭ��
RESULT LoginManager::loginProcess(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char msg[BUFSIZE];
	bool check;
	_user->unPack(&protocol, &buf);

	RESULT result = RT_DEFAULT;
	switch (protocol)
	{
	case CLIENT_REQ_ID_OVERLAP_CHECK:
		reqIdOverlapCheck(_user, buf);
		result = RT_ID_OVERLAP;
		break;
	case CLIENT_REQ_JOIN:
		sprintf(msg, "%s : ���� ��û", _user->getID());
		MsgManager::GetInstance()->DisplayMsg("�α���", msg);
		reqJoin(_user, buf);
		result = RT_JOIN;
		break;
	case CLIENT_REQ_EXIT_JOIN:
		_user->Quepack(SERVER_EXIT_JOIN, 0, 0);
		result = RT_EXIT_JOIN;
	}
	return result;
}

// Ÿ��Ʋ
RESULT LoginManager::TitleProcess(User * _user)
{
	INT64 protocol = 0;
	char buf[BUFSIZE];
	char msg[BUFSIZE];
	bool check = false;
	int choice = 0;

	memset(buf, 0, sizeof(buf));
	memset(msg, 0, sizeof(msg));

	_user->BitunPack(&protocol,&buf);

	RESULT result = RT_DEFAULT;

	// �������� �߰�Ʋ Ÿ��Ʋ�̸�
	if ((protocol&PROTOCOL_LOGIN) == PROTOCOL_LOGIN)
	{
		// �α��� ��û
		if ((protocol&PROTOCOL_REQ_LOGIN) == PROTOCOL_REQ_LOGIN)
		{
			sprintf(msg, "%s : �α��� ��û", _user->getID());
			MsgManager::GetInstance()->DisplayMsg("�α���", msg);
			check = reqLogin(_user, buf);
			if (check == false)
			{
				result = RT_LOGINFAIL;
			}
			result = RT_LOGIN;
		}
		// ���� ��û
		else if ((protocol&PROTOCOL_REQ_JOIN) == PROTOCOL_REQ_JOIN)
		{
			sprintf(msg, "%s : ���� ��û", _user->getID());
			MsgManager::GetInstance()->DisplayMsg("�α���", msg);
			reqJoin(_user, buf);
			result = RT_JOIN;
		}
		// ���̵� �ߺ�Ȯ��
		else if ((protocol&PROTOCOL_REQ_ID_OVERLAP_CHECK) == PROTOCOL_REQ_ID_OVERLAP_CHECK)
		{
			reqIdOverlapCheck(_user, buf);
			result = RT_ID_OVERLAP;
		}
	}


	//// ��������
	//switch (protocol)
	//{
	//case CLIENT_REQ_LOGIN:	// �α��� ��û
	//	sprintf(msg, "%s : �α��� ��û", _user->getID());
	//	MsgManager::GetInstance()->DisplayMsg("�α���", msg);
	//	check = reqLogin(_user, buf);
	//	if (check == false)
	//	{
	//		result = RT_LOGINFAIL;
	//		break;
	//	}
	//	result = RT_LOGIN;
	//	break;
	//case CLIENT_REQ_ID_OVERLAP_CHECK:	// ���̵� �ߺ�üũ ��û
	//	reqIdOverlapCheck(_user, buf);
	//	result = RT_ID_OVERLAP;
	//	break;
	//case CLIENT_REQ_JOIN:	// ȸ������ ��û
	//	sprintf(msg, "%s : ���� ��û", _user->getID());
	//	MsgManager::GetInstance()->DisplayMsg("�α���", msg);
	//	reqJoin(_user, buf);
	//	result = RT_JOIN;
	//	break;
	//default:
	//	break;
	//}

	return result;
}

// Ÿ��Ʋ ȭ��
RESULT LoginManager::logoutMenuChoice(User* _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char msg[BUFSIZE];
	bool check;
	int choice;

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result = RT_DEFAULT;

	// ��������
	switch (protocol)
	{
	case CLIENT_REQ_LOGIN: // �α��� ��û
		sprintf(msg, "%s : �α��� ��û", _user->getID());
		MsgManager::GetInstance()->DisplayMsg("�α���", msg);
		check = reqLogin(_user, buf);
		if (check == false)
		{
			result = RT_LOGINFAIL;
			break;
		}
		result = RT_LOGIN;
		break;
	case CLIENT_JOIN_MENU_CHOICE: // ȸ������ �Ŵ� ��ư ����
		sendprotocol = SERVER_JOIN;
		_user->Quepack(sendprotocol, buf, 0);
		result = RT_JOINMENU;
		break;
	default:
		break;
	}

	return result;
}
