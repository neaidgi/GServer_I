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
	m_stagedata = new StageData();
}
GameDataManager::~GameDataManager()
{
	delete spawndata;
	delete m_dungeon_spawndata;
	delete m_monster_spawndata;
	delete characterdata;
	delete m_monsterdata;
	delete m_stagedata;
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
		return false;
	}
	// ���� ���赵 �ʱ�ȭ
	if (Init_Monster_Data() == false)
	{
		return false;
	}
	// ���� �������� �ʱ�ȭ
	if (Init_Monster_Spawn_Data() == false)
	{
		return false;
	}
	// �������� ���赵 �ʱ�ȭ
	if (Init_Stage_Data() == false)
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

// �������� ���� �ʱ�ȭ
bool GameDataManager::Init_Stage_Data()
{
	// ĳ���� ���� ��������
	StageInfo * origin[STAGE_COUNT];
	for (int i = 0; i < STAGE_COUNT; i++)
	{
		origin[i] = new StageInfo();
	}
	MsgManager::GetInstance()->DisplayMsg("����", "�����������赵 �ε���");
	if (DBManager::GetInstance()->Stage_Req_StageInfo(origin) == false)
	{
		return false;
	}
	if (DBManager::GetInstance()->Stage_Req_StageMonsterInfo(origin) == false)
	{
		return false;
	}

	m_stagedata->SetStageDataOrigin(origin);
	MsgManager::GetInstance()->DisplayMsg("����", "�����������赵 �ε�Ϸ�");
	return true;
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
	case DOG:
		_monster = m_monsterdata->GetDog();
		break;
	case ORCCANNONSOLDIER:
		_monster = m_monsterdata->GetOrcCannonSoldier();
		break;
	case KING_OF_THEAXE:
		_monster = m_monsterdata->GetKingOfTheAxe();
		break;
	case BEAR:
		_monster = m_monsterdata->GetBear();
		break;
	case DINOSAUR:
		_monster = m_monsterdata->GetDinosaur();
		break;
	case DRAGON:
		_monster = m_monsterdata->GetDragon();
		break;
	}
}

// �������� ��ȣ ���� ���赵 ������
void GameDataManager::Stage_Origin_Data(int _stage_num, const StageInfo *& _stage)
{
	switch (_stage_num)
	{
	case DUNGEON_STAGE_NORMAL_1:
		_stage = m_stagedata->GetFirstStage();
		break;
	case DUNGEON_STAGE_NORMAL_2:
		_stage = m_stagedata->GetSecondStage();
		break;
	case DUNGEON_STAGE_NORMAL_3:
		_stage = m_stagedata->GetThirdStage();
		break;
	case DUNGEON_STAGE_BOSS_4:
		break;
	default:
		break;
	}
}
