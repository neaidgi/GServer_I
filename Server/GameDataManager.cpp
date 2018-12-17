#include "GameDataManager.h"

GameDataManager* GameDataManager::Instance = nullptr;

GameDataManager::GameDataManager()
{
	spawndata = new SpawnData();
}
GameDataManager::~GameDataManager()
{
	delete spawndata;
}

void GameDataManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new GameDataManager();
	}
}

GameDataManager * GameDataManager::GetInstance()
{
	return Instance;
}

void GameDataManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}
}

// ���ӵ����� �Ŵ��� �ʱ�ȭ
bool GameDataManager::InitializeManager()
{
	if (spawndata->Init_Spawn_Data() == false)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void GameDataManager::EndManager()
{

}
