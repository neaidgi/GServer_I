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

// ID 중복체크
void LoginManager::reqIdOverlapCheck(User* user, char* _buf)
{
	INT64 protocol = 0;
	int len = 0;
	char id[IDSIZE];
	char msg[BUFSIZE];
	bool check = false;

	memset(id, 0, sizeof(IDSIZE));
	memset(msg, 0, sizeof(BUFSIZE));

	// 아이디 길이
	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	// 아이디
	memcpy(id, _buf, len);


	check = DBManager::GetInstance()->Login_CheckID(id);

	if (!check)
	{
		user->setID(id);
	}
	sprintf(msg, "%s : %s", id, check ? "중복" : "중복아님");
	MsgManager::GetInstance()->DisplayMsg("로그인", msg);

	protocol = user->BitPackProtocol(protocol, PROTOCOL_TITLE, PROTOCOL_LOGIN, PROTOCOL_ID_OVERLAP_CHECK);
	user->Quepack(protocol, &check, sizeof(bool));
}

// 회원가입 결과
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

	// 아이디 길이
	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	// 아이디
	memcpy(id, _buf, len);
	_buf += len;

	// 비밀번호 길이
	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	// 비밀번호
	memcpy(pw, _buf, len);

	// 추가했음 : 기존에 입력했던 중복된 아이디가 아니라면 회원가입 취소
	if (strcmp(id, user->getID()))
	{
		result = false;
		protocol = user->BitPackProtocol(protocol, PROTOCOL_TITLE, PROTOCOL_LOGIN, PROTOCOL_JOIN_RESULT);
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


	protocol = user->BitPackProtocol(protocol, PROTOCOL_TITLE, PROTOCOL_LOGIN, PROTOCOL_JOIN_RESULT);
	user->Quepack(protocol, &result, sizeof(bool));

	sprintf(tempbuf, "%s [회원가입].", user->getID());
	LogManager::GetInstance()->LogWrite(tempbuf);
	MsgManager::GetInstance()->DisplayMsg("로그인", tempbuf);
}

// 로그인 결과
bool LoginManager::reqLogin(User* user, char* _buf)
{
	UINT64 protocol = 0;
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
		sprintf(tempbuf, "%s [로그인].", user->getID());
	}
	else
	{
		sprintf(tempbuf, "[로그인 실패]");
	}

	MsgManager::GetInstance()->DisplayMsg("로그인", tempbuf);
	protocol = user->BitPackProtocol(protocol, PROTOCOL_TITLE, PROTOCOL_LOGIN, PROTOCOL_LOGIN_RESULT);
	user->Quepack(protocol, &result, sizeof(bool));

	return result;
}

// 타이틀
RESULT LoginManager::TitleProcess(User * _user)
{
	UINT64 protocol = 0;
	UINT64 compartrprotocol = 0;
	UINT64 tempprotocol = 0;
	char buf[BUFSIZE];
	char msg[BUFSIZE];
	bool check = false;
	int choice = 0;

	memset(buf, 0, sizeof(buf));
	memset(msg, 0, sizeof(msg));

	_user->BitunPack(protocol, &buf);

	RESULT result = RT_DEFAULT;

	compartrprotocol = PROTOCOL_LOGIN;

	tempprotocol = 0;

	tempprotocol = protocol & compartrprotocol;
	switch (tempprotocol)
	{
	case PROTOCOL_LOGIN: // 중간틀 로그인이면
		tempprotocol = protocol & PROTOCOL_CLIENT_LOGIN_MENU_COMPART;
		switch (tempprotocol)
		{
		case PROTOCOL_REQ_LOGIN: // 로그인 요청
			sprintf(msg, "%s : 로그인 요청", _user->getID());
			MsgManager::GetInstance()->DisplayMsg("로그인", msg);
			check = reqLogin(_user, buf);
			if (check == false)
			{
				result = RT_LOGINFAIL;
			}
			else
			{
				result = RT_LOGIN;
			}
			break;
		case PROTOCOL_REQ_JOIN: // 가입 요청
			sprintf(msg, "%s : 가입 요청", _user->getID());
			MsgManager::GetInstance()->DisplayMsg("로그인", msg);
			reqJoin(_user, buf);
			result = RT_JOIN;
			break;
		case PROTOCOL_REQ_ID_OVERLAP_CHECK:	// 아이디 중복확인
			reqIdOverlapCheck(_user, buf);
			result = RT_ID_OVERLAP;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	return result;
}


//// 회원가입 화면
//RESULT LoginManager::loginProcess(User * _user)
//{
//	PROTOCOL protocol;
//	char buf[BUFSIZE];
//	char msg[BUFSIZE];
//	bool check;
//	_user->unPack(&protocol, &buf);
//
//	RESULT result = RT_DEFAULT;
//	switch (protocol)
//	{
//	case CLIENT_REQ_ID_OVERLAP_CHECK:
//		reqIdOverlapCheck(_user, buf);
//		result = RT_ID_OVERLAP;
//		break;
//	case CLIENT_REQ_JOIN:
//		sprintf(msg, "%s : 가입 요청", _user->getID());
//		MsgManager::GetInstance()->DisplayMsg("로그인", msg);
//		reqJoin(_user, buf);
//		result = RT_JOIN;
//		break;
//	case CLIENT_REQ_EXIT_JOIN:
//		_user->Quepack(SERVER_EXIT_JOIN, 0, 0);
//		result = RT_EXIT_JOIN;
//	}
//	return result;
//}

//// 타이틀 화면
//RESULT LoginManager::logoutMenuChoice(User* _user)
//{
//	PROTOCOL protocol;
//	char buf[BUFSIZE];
//	char msg[BUFSIZE];
//	bool check;
//	int choice;
//
//	_user->unPack(&protocol, &buf);
//
//	PROTOCOL sendprotocol;
//
//	RESULT result = RT_DEFAULT;
//
//	// 수정했음
//	switch (protocol)
//	{
//	case CLIENT_REQ_LOGIN: // 로그인 요청
//		sprintf(msg, "%s : 로그인 요청", _user->getID());
//		MsgManager::GetInstance()->DisplayMsg("로그인", msg);
//		check = reqLogin(_user, buf);
//		if (check == false)
//		{
//			result = RT_LOGINFAIL;
//			break;
//		}
//		result = RT_LOGIN;
//		break;
//	case CLIENT_JOIN_MENU_CHOICE: // 회원가입 매뉴 버튼 누름
//		sendprotocol = SERVER_JOIN;
//		_user->Quepack(sendprotocol, buf, 0);
//		result = RT_JOINMENU;
//		break;
//	default:
//		break;
//	}
//
//	return result;
//}
