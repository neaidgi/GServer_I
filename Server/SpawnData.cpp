#include "SpawnData.h"

// class SpawnData()
SpawnData::SpawnData()
{
	character_spawnpos_count = 0;
}

SpawnData::~SpawnData()
{
}

void SpawnData::SetCharacterSpawnPos(Vector3* _v, int _count)
{
	for (int i = 0; i < _count; i++)
	{
		character_spawn_pos[i] = _v[i];
	}
}

const Vector3 * SpawnData::Character_Spawn_PosData()
{
	return character_spawn_pos;
}

// class DungeonSpawnData()

DungeonSpawnData::DungeonSpawnData()
{
	m_dungeon_spawnpos_count = 0;
}

DungeonSpawnData::~DungeonSpawnData()
{
}

void DungeonSpawnData::SetDungeonSpawnPos(Vector3 * _v, int _count)
{
	for (int i = 0; i < _count; i++)
	{
		m_dungeon_spawn_pos[i] = _v[i];
	}
}

const Vector3 * DungeonSpawnData::Dungeon_Spawn_PosData()
{
	return m_dungeon_spawn_pos;
}


// class MonsterSpawnData()

MonsterSpawnData::MonsterSpawnData()
{
	m_dungeon_spawnpos_count = 0;
}

MonsterSpawnData::~MonsterSpawnData()
{

}

void MonsterSpawnData::SetMonsterSpawnPos(Vector3 * _v, int _count)
{
	for (int i = 0; i < _count; i++)
	{
		m_monster_spawn_pos[i] = _v[i];
	}
}

const Vector3 * MonsterSpawnData::Monster_Spawn_PosData()
{
	return m_monster_spawn_pos;
}
