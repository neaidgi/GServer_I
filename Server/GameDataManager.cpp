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

// ���ӵ����� �Ŵ��� �ʱ�ȭ
bool GameDataManager::InitializeManager()
{
	MsgManager::GetInstance()->DisplayMsg("����","���ӵ����� �ε���");
	if (Init_Spawn_Data() == false)
	{
		return false;
	}
	
	// ĳ���� ���赵 �ʱ�ȭ
	if (Init_Character_Data() == false)
	{
		// �α�
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

	characterdata->SetCharacterOrigin(origin);
	MsgManager::GetInstance()->DisplayMsg("����", "ĳ���ͼ��赵 �ε�Ϸ�");
	return true;
}


// �÷��̾� ������ġ �迭
void GameDataManager::Character_SpawnPos_Vector(Vector3 * _pos)
{
	memcpy(_pos, spawndata->Character_Spawn_PosData(), sizeof(Vector3));

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
