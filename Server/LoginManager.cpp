#include "LoginManager.h"
#include "LogManager.h"
#include "DBManager.h"

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
	int len;
	char id[IDSIZE];
	bool check = false;

	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);

	memcpy(id, _buf, len);


	check = DBManger::GetInstance()->Login_CheckID(id);

	if (!check)
	{
		user->setID(id);
	}

	user->pack(SERVER_ID_OVERLAP_CHECK, &check, sizeof(bool));
}

void LoginManager::reqJoin(User* user, char* _buf)
{
	if (userCount >= USERCOUNT)
		return;

	int len;
	char id[IDSIZE];
	char pw[IDSIZE];
	char tempbuf[IDSIZE + 10];
	bool result = false;

	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	memcpy(id, _buf, len);
	_buf += len;

	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);
	memcpy(pw, _buf, len);

	// 추가했음 : 기존에 입력했던 중복된 아이디가 아니라면 회원가입 취소
	if (strcmp(id, user->getID()))
	{
		result = false;
		user->pack(SERVER_JOIN_SUCCESS, &result, sizeof(bool));
		return;
	}

	UserInfo* info = new UserInfo();
	strcpy(info->id, user->getID());
	strcpy(info->pw, pw);

	result = DBManger::GetInstance()->Login_reqJoin(info->id, info->pw, NULL);

	if (result)
	{
		user->setPW(info->pw);
	}

	//userList[userCount++] = info;
	user->pack(SERVER_JOIN_SUCCESS, &result, sizeof(bool));

	sprintf(tempbuf, "%s [회원가입].", user->getID());
	LogManager::GetInstance()->LogWrite(tempbuf);
}

bool LoginManager::reqLogin(User* user, char* _buf)
{
	int len;
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

	//const UserInfo* info = getUser(id);

	bool result;

	result = DBManger::GetInstance()->Login_reqLogin(id, pw);

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
	
	user->include_wset = true;
	user->pack(SERVER_LOGIN_SUCCESS, &result, sizeof(bool));
	
	return result;
}

// 
RESULT LoginManager::loginProcess(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	bool check;
	_user->unPack(&protocol, &buf);

	RESULT result;
	switch (protocol)
	{
	case CLIENT_REQ_ID_OVERLAP_CHECK:
		reqIdOverlapCheck(_user, buf);
		result = RT_ID_OVERLAP;
		break;
	case CLIENT_REQ_JOIN:
		reqJoin(_user, buf);
		result = RT_JOIN;
		break;
	case CLIENT_REQ_EXIT_JOIN:
		
		_user->pack(SERVER_EXIT_JOIN, 0, 0);

		result = RT_EXIT_JOIN;
	}
	return result;
}

RESULT LoginManager::logoutMenuChoice(User* _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	bool check;
	int choice;

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_REQ_LOGIN:
		check = reqLogin(_user, buf);
		if (check == false)
		{
			result = RT_LOGINFAIL;
			break;
		}
		result = RT_LOGIN;
		break;
	case CLIENT_LOGOUT_MENU_CHOICE:
		
		memcpy(&choice, buf, sizeof(int));
		
		if (choice == 2)
		{
			sendprotocol = SERVER_JOIN;
			_user->pack(sendprotocol, buf, 0);
			_user->include_wset = true;
			result = RT_JOINMENU;
		}
		else
		{
			result = RT_LOGINMENU;
		}
		break;
	default:
		break;
	}

	return result;
}

RESULT LoginManager::loginMenuChoice(User* _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];

	_user->unPack(&protocol, &buf);
	int choice;
	memcpy(&choice, buf, sizeof(int));

	char tempbuf[IDSIZE + 10];	//문자열만들어줄것

	RESULT result;
	PROTOCOL sendprotocol;
	switch (choice)
	{
	case LoginMenu.TENDER:
		sendprotocol = SERVER_TENDER;
		_user->pack(sendprotocol, buf, 0);
		_user->include_wset = true;
		result = RT_TENDER;
		break;
	case LoginMenu.LEAVE:
		{
			bool result;

			char id[IDSIZE];

			strcpy(id, _user->getID());
			result = DBManger::GetInstance()->Login_reqLeave(id);

			_user->setID("");
			_user->setPW("");
		
			if (result)
			{
				sprintf(tempbuf, "[회원탈퇴 성공]");
			}
			else
			{
				sprintf(tempbuf, "[회원탈퇴 실패]");
			}
		}
		sendprotocol = SERVER_LEAVE;
		_user->setLogout();

		_user->pack(sendprotocol, buf, 0);
		_user->include_wset = true;
		result = RT_MEMBER_LEAVE;
		break;
	case LoginMenu.LOGOUT:
		sprintf(tempbuf, "%s [로그아웃].", _user->getID());
		LogManager::GetInstance()->LogWrite(tempbuf);

		sendprotocol = SERVER_LOGOUT;
		_user->setLogout();

		_user->pack(sendprotocol, buf, 0);
		_user->include_wset = true;
		result = RT_LOGOUT;
		break;
	case LoginMenu.EXIT:
		sprintf(tempbuf, "%s [종료].", _user->getID());
		LogManager::GetInstance()->LogWrite((char*)_user->getID());
		sendprotocol = EXIT;
		result = RT_USER_DISCONNECT;
	}
	return result;
}
