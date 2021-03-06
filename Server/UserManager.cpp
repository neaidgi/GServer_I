#include "UserManager.h"

UserManager* UserManager::Instance = nullptr;

UserManager::UserManager()
{

}
UserManager::~UserManager()
{
	User* target = nullptr;

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);
		delete target;
	}
}

void UserManager::CreateInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}

	Instance = new UserManager();
}

UserManager* UserManager::GetInstance()
{
	return Instance;
}

void UserManager::DestroyInstance()
{
	delete Instance;
}

bool UserManager::InitializeManager()
{
	return true;
}

void UserManager::EndManager()
{

}

User* UserManager::addUser(SOCKET _sock)
{
	User* newUser = nullptr;

	if (ConnectUserlist.size() < CLIENTCOUNT &&
		_sock != SOCKET_ERROR)
	{
		SOCKADDR_IN addr;
		int addrlen = sizeof(addr);

		getpeername(_sock, (SOCKADDR*)&addr, &addrlen);

		newUser = new User(_sock, addr);
		ConnectUserlist.push_back(newUser);
	}
	return newUser;
}

void UserManager::removeUser(User* _user)
{
	CThreadSync cs;
	// 유저 찾아서 삭제
	ConnectUserlist.remove(_user);
	delete _user;
	_user = nullptr;
}

User* UserManager::getUser(SOCKET _sock)
{
	CThreadSync cs;
	User* target = nullptr;

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);
		if (target != nullptr)
		{
			if (target->getSocket() == _sock && target != nullptr)
			{
				break;
			}
		}
	}
	return target;
}

User* UserManager::getUser(char* _id)
{
	CThreadSync cs;
	User* target = nullptr;

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);
		if (target != nullptr)
		{
			if (target->getID() == _id)
			{
				break;
			}
		}
	}
	return target;
}

User * UserManager::getUserCode(char * _code)
{
	CThreadSync cs;
	User* target = nullptr;

	char msg[BUFSIZE];

	// 메세지
	memset(msg, 0, sizeof(msg));

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);

		if (target != nullptr)
		{
			if (strcmp(target->GetCurCharacter()->GetCharacter_Code(), _code) == false && target->isIngame())
			{
				sprintf(msg, "getUserCode :: 유저 ID : [%s] 캐릭터 코드 : [%s] ", target->getID(), target->GetCurCharacter()->GetCharacter_Code());
				MsgManager::GetInstance()->DisplayMsg("INFO", msg);
				break;
			}
		}

	}
	return target;
}

User * UserManager::getUserCode(const char * _code)
{
	CThreadSync cs;
	User* target = nullptr;

	char msg[BUFSIZE];

	// 메세지
	memset(msg, 0, sizeof(msg));

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);

		if (target != nullptr)
		{
			if (strcmp(target->GetCurCharacter()->GetCharacter_Code(), _code) == false && target->isIngame())
			{
				sprintf(msg, "getUserCode :: 유저 ID : [%s] 캐릭터 코드 : [%s] ", target->getID(), target->GetCurCharacter()->GetCharacter_Code());
				MsgManager::GetInstance()->DisplayMsg("INFO", msg);
				break;
			}
		}
	}
	return target;
}

bool UserManager::isUser(User * _user)
{
	CThreadSync cs;
	User* target = nullptr;

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);

		if (target == _user)
		{
			return true;
		}
	}
	return false;
}

void UserManager::startSearch()
{
	// 리스트 시작부 세팅
	save = ConnectUserlist.begin();
}

bool UserManager::searchData(User *& _user)
{
	CThreadSync cs;
	// 유저 하나씩 꺼내서 save에 넣음
	if (save != ConnectUserlist.end())
	{
		_user = *save;
		++save;
		return true;
	}
	else
	{
		return false;
	}
}
