#include "GameManager.h"

GameManager* GameManager::Instance = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new GameManager();
	}
}

GameManager* GameManager::GetInstance()
{
	return Instance;
}

void GameManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool GameManager::InitializeManager()
{
	if (Initialize_Iocp() == false)
	{
		ErrorManager::GetInstance()->err_quit("IOCP Init fail");
		return false;
	}
	return true;
}

void GameManager::EndManager()
{

}

bool GameManager::acceptProcess(User * _user)
{
	if (_user == nullptr)
	{
		return false;
	}

	_user->InitState();

	if (_user->IOCP_RecvMsg() == false)
	{
		return false;
	}
	return true;
}

bool GameManager::recvProcess(User* _user)
{
	if (_user == nullptr)
	{
		return false;
	}

	if (_user->getState()->Read(_user) == false)
	{
		_user->stop();
		UserManager::GetInstance()->removeUser(_user);
		return false;
	}
	return true;
}

bool GameManager::sendProcess(User* _user)
{
	if (_user == nullptr)
	{
		return false;
	}

	if (_user->getState()->Write(_user) == false)
	{
		_user->stop();
		UserManager::GetInstance()->removeUser(_user);
		return false;
	}

	return true;
}

