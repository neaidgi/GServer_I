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

// ���ӵ����� �Ŵ��� �ʱ�ȭ
bool GameDataManager::InitializeManager()
{
	MsgManager::GetInstance()->DisplayMsg("����","���ӵ����� �ε���");
	if (spawndata->Init_Spawn_Data() == false)
	{
		return false;
	}
	else
	{
		MsgManager::GetInstance()->DisplayMsg("����","���ӵ����� �ε�Ϸ�");
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
