#include "GameDataManager.h"
#include "ErrorManager.h"
#include "MsgManager.h"

GameDataManager* GameDataManager::Instance = nullptr;

GameDataManager::GameDataManager()
{
	spawndata = new SpawnData();
	m_dungeon_spawndata = new DungeonSpawnData();
	characterdata = new CharacterData();
	m_monsterdata = new MonsterData();
	m_monster_spawndata = new MonsterSpawnData();
}
GameDataManager::~GameDataManager()
{
	delete spawndata;
	delete m_dungeon_spawndata;
	delete m_monster_spawndata;
	delete characterdata;
	delete m_monsterdata;
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
	// 던전 스폰위치 초기화
	if (Init_Dungeon_Spawn_Data() == false)
	{
		return false;
	}
	
	// 캐릭터 설계도 초기화
	if (Init_Character_Data() == false)
	{
		// 로그
		return false;
	}
	// 몬스터 설계도 초기화
	if (Init_Monster_Data() == false)
	{
		return false;
	}
	if (Init_Monster_Spawn_Data() == false)
	{
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

// 캐릭터 설계도 초기화
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
	if (DBManager::GetInstance()->Character_Req_AttackInfo(origin) == false)
	{
		return false;
	}

	characterdata->SetCharacterOrigin(origin);
	MsgManager::GetInstance()->DisplayMsg("메인", "캐릭터설계도 로드완료");
	return true;
}

// 몬스터 설계도 초기화
bool GameDataManager::Init_Monster_Data()
{
	// 캐릭터 정보 가져오기
	Monster * origin[MAXMONSTERORIGIN];
	for (int i = 0; i < MAXMONSTERORIGIN; i++)
	{
		origin[i] = new Monster();
	}
	MsgManager::GetInstance()->DisplayMsg("메인", "몬스터설계도 로드중");
	if (DBManager::GetInstance()->Character_Req_MonsterInfo(origin) == false)
	{
		return false;
	}
	if (DBManager::GetInstance()->Monster_Req_AttackInfo(origin) == false)
	{
		return false;
	}

	m_monsterdata->SetMonsterOrigin(origin);
	MsgManager::GetInstance()->DisplayMsg("메인", "몬스터설계도 로드완료");
	return true;
}

// 던전 스폰위치 저장
bool GameDataManager::Dungeon_SpawnPos_Load()
{
	DBManager* db = DBManager::GetInstance();

	Vector3 pos[DUNGEON_SPAWNPOS_MAXCOUNT];
	int count = 0;

	if (db->Charactor_Req_DungeonSpawnPos(pos, count) == false)
	{
		return false;
	}

	Vector3 stagepos[DUNGEON_SPAWNPOS_MAXCOUNT];
	int stage_count = 0;
	if (db->Charactor_Req_DungeonStageSpawnPos(stagepos, stage_count) == false)
	{
		return false;
	}
	else
	{
		m_dungeon_spawndata->SetDungeonSpawnPos(pos, count);
		m_dungeon_spawndata->SetDungeonStageSpawnPos(stagepos, count);
		return true;
	}
}

// 던전 스폰위치 초기화
bool GameDataManager::Init_Dungeon_Spawn_Data()
{
	if (Dungeon_SpawnPos_Load() == false)
	{
		return false;
	}
}

// 몬스터 스폰위치 저장
bool GameDataManager::Monster_SpawnPos_Load()
{
	Vector3 pos[MONSTER_SPAWNPOS_MAXCOUNT];
	int count = 0;

	if (DBManager::GetInstance()->Charactor_Req_MonsterSpawnPos(pos, count) == false)
	{
		return false;
	}
	else
	{
		m_monster_spawndata->SetMonsterSpawnPos(pos, count);
		return true;
	}
}

// 몬스터 스폰위치 초기화
bool GameDataManager::Init_Monster_Spawn_Data()
{
	if (Monster_SpawnPos_Load() == false)
	{
		return false;
	}
}

// 플레이어 스폰위치 배열
void GameDataManager::Character_SpawnPos_Vector(Vector3 * _pos)
{
	memcpy(_pos, spawndata->Character_Spawn_PosData(), sizeof(Vector3) * 6);

}

// 던전 스폰위치 배열
void GameDataManager::Dungeon_SpawnPos_Vecotr(Vector3 * _pos)
{
	memcpy(_pos, m_dungeon_spawndata->Dungeon_Spawn_PosData(), sizeof(Vector3) * 4);
}

// 던전 몬스터 스폰위치 배열
void GameDataManager::Dungeon_Monster_SpawnPos_Vector(Vector3 * _pos)
{
	memcpy(_pos, m_monster_spawndata->Monster_Spawn_PosData(), sizeof(Vector3) * 3);

}

// 던전 스테이지 스폰위치 배열
void GameDataManager::Dungeon_Stage_SpawnPos_Vecotr(Vector3 * _pos)
{
	memcpy(_pos, m_dungeon_spawndata->Dungeon_Stage_Spawn_PosData(), sizeof(Vector3) * 4);
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

// 몬스터 코드에 따라 설계도 가져옴
void GameDataManager::Monster_Origin_Data(int _monstercode, const Monster *& _monster)
{
	switch (_monstercode)
	{
	case SPIDER:
		_monster = m_monsterdata->GetSpider();
		break;
	case WORM:
		_monster = m_monsterdata->GetWorm();
		break;
	case BOSS_SPIDER:
		_monster = m_monsterdata->GetBossSpider();
		break;
	}
}
