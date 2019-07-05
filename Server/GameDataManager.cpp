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

// ���ӵ����� �Ŵ��� �ʱ�ȭ
bool GameDataManager::InitializeManager()
{
	MsgManager::GetInstance()->DisplayMsg("����","���ӵ����� �ε���");
	if (Init_Spawn_Data() == false)
	{
		return false;
	}
	// ���� ������ġ �ʱ�ȭ
	if (Init_Dungeon_Spawn_Data() == false)
	{
		return false;
	}
	
	// ĳ���� ���赵 �ʱ�ȭ
	if (Init_Character_Data() == false)
	{
		// �α�
		return false;
	}
	// ���� ���赵 �ʱ�ȭ
	if (Init_Monster_Data() == false)
	{
		return false;
	}
	if (Init_Monster_Spawn_Data() == false)
	{
		return false;
	}
	MsgManager::GetInstance()->DisplayMsg("����", "���ӵ����� �ε�Ϸ�");
	return true;
}

void GameDataManager::EndManager()
{

}

// �÷��̾� ���� ��ġ DB���� ������
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

// ���������� Ŭ���� �ʱ�ȭ
bool GameDataManager::Init_Spawn_Data()
{
	if (Player_SpawnPos_Load() == false)
	{
		return false;
	}
}

// ĳ���� ���赵 �ʱ�ȭ
bool GameDataManager::Init_Character_Data()
{
	// ĳ���� ���� ��������
	Character * origin[MAXCHARACTERORIGIN];
	for (int i = 0; i < MAXCHARACTERORIGIN; i++)
	{
		origin[i] = new Character();
	}
	MsgManager::GetInstance()->DisplayMsg("����", "ĳ���ͼ��赵 �ε���");
	if (DBManager::GetInstance()->Character_Req_CharacterInfo(origin) == false)
	{
		return false;
	}
	if (DBManager::GetInstance()->Character_Req_AttackInfo(origin) == false)
	{
		return false;
	}

	characterdata->SetCharacterOrigin(origin);
	MsgManager::GetInstance()->DisplayMsg("����", "ĳ���ͼ��赵 �ε�Ϸ�");
	return true;
}

// ���� ���赵 �ʱ�ȭ
bool GameDataManager::Init_Monster_Data()
{
	// ĳ���� ���� ��������
	Monster * origin[MAXMONSTERORIGIN];
	for (int i = 0; i < MAXMONSTERORIGIN; i++)
	{
		origin[i] = new Monster();
	}
	MsgManager::GetInstance()->DisplayMsg("����", "���ͼ��赵 �ε���");
	if (DBManager::GetInstance()->Character_Req_MonsterInfo(origin) == false)
	{
		return false;
	}
	if (DBManager::GetInstance()->Monster_Req_AttackInfo(origin) == false)
	{
		return false;
	}

	m_monsterdata->SetMonsterOrigin(origin);
	MsgManager::GetInstance()->DisplayMsg("����", "���ͼ��赵 �ε�Ϸ�");
	return true;
}

// ���� ������ġ ����
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

// ���� ������ġ �ʱ�ȭ
bool GameDataManager::Init_Dungeon_Spawn_Data()
{
	if (Dungeon_SpawnPos_Load() == false)
	{
		return false;
	}
}

// ���� ������ġ ����
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

// ���� ������ġ �ʱ�ȭ
bool GameDataManager::Init_Monster_Spawn_Data()
{
	if (Monster_SpawnPos_Load() == false)
	{
		return false;
	}
}

// �÷��̾� ������ġ �迭
void GameDataManager::Character_SpawnPos_Vector(Vector3 * _pos)
{
	memcpy(_pos, spawndata->Character_Spawn_PosData(), sizeof(Vector3) * 6);

}

// ���� ������ġ �迭
void GameDataManager::Dungeon_SpawnPos_Vecotr(Vector3 * _pos)
{
	memcpy(_pos, m_dungeon_spawndata->Dungeon_Spawn_PosData(), sizeof(Vector3) * 4);
}

// ���� ���� ������ġ �迭
void GameDataManager::Dungeon_Monster_SpawnPos_Vector(Vector3 * _pos)
{
	memcpy(_pos, m_monster_spawndata->Monster_Spawn_PosData(), sizeof(Vector3) * 3);

}

// ���� �������� ������ġ �迭
void GameDataManager::Dungeon_Stage_SpawnPos_Vecotr(Vector3 * _pos)
{
	memcpy(_pos, m_dungeon_spawndata->Dungeon_Stage_Spawn_PosData(), sizeof(Vector3) * 4);
}

// ���������� ���赵 ������
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

// ���� �ڵ忡 ���� ���赵 ������
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
