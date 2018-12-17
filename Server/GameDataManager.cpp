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

// 게임데이터 매니저 초기화
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

void GameDataManager::Character_SpawnPos_Vector(Vector3 * _pos)
{
	_pos = spawndata->Character_Spawn_PosData();
}
