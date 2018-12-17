#include "SpawnData.h"

SpawnData::SpawnData()
{
	character_spawnpos_count = 0;
}

SpawnData::~SpawnData()
{
}

const Vector3 * SpawnData::Character_Spawn_PosData()
{
	return character_spawn_pos;
}

// 플레이어 스폰위치 DB에서 받아옴
bool SpawnData::Player_SpawnPos_Load()
{
	DBManager* db = DBManager::GetInstance();

	if (db->Charactor_Req_CharacterSpawnPos(character_spawn_pos, character_spawnpos_count) == false)
	{
		return false;
	}
}

// 스폰데이터 클래스 초기화
bool SpawnData::Init_Spawn_Data()
{
	if (Player_SpawnPos_Load() == false)
	{
		return false;
	}
}
