#ifndef _SPAWNDATA_H_
#define _SPAWNDATA_H_

#include "DBManager.h"
#include "Global.h"

class SpawnData {
private:
	// ĳ���� ������ġ
	Vector3 character_spawn_pos[CHARACTER_SPAWNPOS_MAXCOUNT];
	// ĳ���� ������ġ ī��Ʈ
	int character_spawnpos_count;
public:
	SpawnData();
	~SpawnData();
	
	void SetCharacterSpawnPos(Vector3* _v, int _count);
	void SetCharacterSpawnCount(int _count) { character_spawnpos_count = _count; }

	// ����������
	const Vector3* Character_Spawn_PosData();
};


class DungeonSpawnData {
private:
	// ���� ĳ���� ������ġ
	Vector3 m_dungeon_spawn_pos[DUNGEON_SPAWNPOS_MAXCOUNT];
	// ���� ĳ���� ������ġ ī��Ʈ
	int m_dungeon_spawnpos_count;
	// ���� �������� ĳ���� ������ġ
	Vector3 m_dungeon_stage_spawn_pos[DUNGEON_SPAWNPOS_MAXCOUNT];
	// ���� �������� ĳ���� ������ġ ī��Ʈ
	int m_dungeon_stage_spawnpos_count;
public:
	DungeonSpawnData();
	~DungeonSpawnData();

	void SetDungeonSpawnPos(Vector3* _v, int _count);
	void SetDungeonSpawnCount(int _count) { m_dungeon_spawnpos_count = _count; }

	void SetDungeonStageSpawnPos(Vector3* _v, int _count);
	void SetDungeonStageSpawnCount(int _count) { m_dungeon_stage_spawnpos_count = _count; }

	// ����������
	const Vector3* Dungeon_Spawn_PosData();
	const Vector3* Dungeon_Stage_Spawn_PosData();
};


class MonsterSpawnData {
private:
	// ���� ������ġ
	Vector3 m_monster_spawn_pos[MONSTER_SPAWNPOS_MAXCOUNT];
	// ���� ������ġ ī��Ʈ
	int m_dungeon_spawnpos_count;
public:
	MonsterSpawnData();
	~MonsterSpawnData();

	void SetMonsterSpawnPos(Vector3* _v, int _count);
	void SetMonsterSpawnCount(int _count) { m_dungeon_spawnpos_count = _count; }

	// ����������
	const Vector3* Monster_Spawn_PosData();
};
#endif