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

bool SpawnData::Player_SpawnPos_Load()
{
	DBManager* db = DBManager::GetInstance();

	if (db->Charactor_Req_CharacterSpawnPos(character_spawn_pos, character_spawnpos_count) == false)
	{
		return false;
	}
	else
	{

	}
}

bool SpawnData::Init_Spawn_Data()
{
	if (Player_SpawnPos_Load() == false)
	{
		return false;
	}
}
