#include "NetworkDataManager.h"

NetworkDataManager* NetworkDataManager::Instance = nullptr;

NetworkDataManager::NetworkDataManager()
{
}

NetworkDataManager::~NetworkDataManager()
{
}


void NetworkDataManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new NetworkDataManager();
	}
}

NetworkDataManager* NetworkDataManager::GetInstance()
{
	return Instance;
}

void NetworkDataManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool NetworkDataManager::InitializeManager()
{
	return true;
}

void NetworkDataManager::EndManager()
{

}

