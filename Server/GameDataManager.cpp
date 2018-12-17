#include "GameDataManager.h"
#include "ErrorManager.h"
#include "MsgManager.h"

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
	MsgManager::GetInstance()->DisplayMsg("메인","게임데이터 로드중");
	if (spawndata->Init_Spawn_Data() == false)
	{
		return false;
	}
	else
	{
		MsgManager::GetInstance()->DisplayMsg("메인","게임데이터 로드완료");
		return true;
	}
}

void GameDataManager::EndManager()
{

}

void GameDataManager::Character_SpawnPos_Vector(Vector3 * _pos)
{
	memcpy(_pos, spawndata->Character_Spawn_PosData(), sizeof(Vector3));
}
