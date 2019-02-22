#include "GameDataManager.h"
#include "ErrorManager.h"
#include "MsgManager.h"

GameDataManager* GameDataManager::Instance = nullptr;

GameDataManager::GameDataManager()
{
	spawndata = new SpawnData();
	characterdata = new CharacterData();
}
GameDataManager::~GameDataManager()
{
	delete spawndata;
	delete characterdata;
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
	if (Init_Spawn_Data() == false)
	{
		return false;
	}
	
	// 캐릭터 설계도 초기화
	if (Init_Character_Data() == false)
	{
		// 로그
		return false;
	}

	MsgManager::GetInstance()->DisplayMsg("메인", "게임데이터 로드완료");
	return true;
}

void GameDataManager::EndManager()
{

}

// 플레이어 스폰 위치 DB에서 가져옴
bool GameDataManager::Player_SpawnPos_Load()
{
	DBManager* db = DBManager::GetInstance();

	Vector3 pos[CHARACTER_SPAWNPOS_MAXCOUNT];
	int count = 0;

	if (db->Charactor_Req_CharacterSpawnPos(pos, count) == false)
	{
		return false;
	}
	else
	{ 
		spawndata->SetCharacterSpawnPos(pos, count);
		return true;
	}
}

// 스폰데이터 클래스 초기화
bool GameDataManager::Init_Spawn_Data()
{
	if (Player_SpawnPos_Load() == false)
	{
		return false;
	}
}

bool GameDataManager::Init_Character_Data()
{
	// 캐릭터 정보 가져오기
	Character * origin[MAXCHARACTERORIGIN];
	for (int i = 0; i < MAXCHARACTERORIGIN; i++)
	{
		origin[i] = new Character();
	}
	MsgManager::GetInstance()->DisplayMsg("메인", "캐릭터설계도 로드중");
	if (DBManager::GetInstance()->Character_Req_CharacterInfo(origin) == false)
	{
		return false;
	}

	characterdata->SetCharacterOrigin(origin);
	MsgManager::GetInstance()->DisplayMsg("메인", "캐릭터설계도 로드완료");
	return true;
}


// 플레이어 스폰위치 배열
void GameDataManager::Character_SpawnPos_Vector(Vector3 * _pos)
{
	memcpy(_pos, spawndata->Character_Spawn_PosData(), sizeof(Vector3));

}
// 직업에따라 설계도 가져옴
void GameDataManager::Character_Origin_Data(int _jobcode, const Character*& _job)
{
	switch (_jobcode)
	{
	case TANKER:
		_job = characterdata->GetTanker();
		break;
	case WARRIOR:
		_job = characterdata->GetWarrior();
		break;
	case MAGICIAN:
		_job = characterdata->GetMagician();
		break;
	case GUNNER:
		_job = characterdata->GetGunner();
		break;
	}
}
