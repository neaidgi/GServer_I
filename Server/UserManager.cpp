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
	CriticalSectionManager::GetInstance()->Enter();

	// ���� ã�Ƽ� ����
	ConnectUserlist.remove(_user);
	delete _user;
	_user = nullptr;

	CriticalSectionManager::GetInstance()->Leave();
}

User* UserManager::getUser(SOCKET _sock)
{
	User* target = nullptr;

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);
		if (target->getSocket() == _sock)
		{
			break;
		}
	}
	return target;
}

User* UserManager::getUser(char* _id)
{
	User* target = nullptr;

	for (std::list<User*>::iterator i = ConnectUserlist.begin(); i != ConnectUserlist.end(); ++i)
	{
		target = (*i);
		if (target->getID() == _id)
		{
			break;
		}
	}
	return target;
}

bool UserManager::isUser(User * _user)
{
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
	// ����Ʈ ���ۺ� ����
	save = ConnectUserlist.begin();
}

bool UserManager::searchData(User *& _user)
{
	// ���� �ϳ��� ������ save�� ����
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
