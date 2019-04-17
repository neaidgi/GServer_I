#ifndef _SPAWNDATA_H_
#define _SPAWNDATA_H_

#include "DBManager.h"
#include "Global.h"

class SpawnData {
private:
	// 캐릭터 스폰위치
	Vector3 character_spawn_pos[CHARACTER_SPAWNPOS_MAXCOUNT];
	// 캐릭터 스폰위치 카운트
	int character_spawnpos_count;
public:
	SpawnData();
	~SpawnData();
	
	void SetCharacterSpawnPos(Vector3* _v, int _count);
	void SetCharacterSpawnCount(int _count) { character_spawnpos_count = _count; }

	// 접근지정자
	const Vector3* Character_Spawn_PosData();
};


class DungeonSpawnData {
private:
	// 던전 캐릭터 스폰위치
	Vector3 m_dungeon_spawn_pos[DUNGEON_SPAWNPOS_MAXCOUNT];
	// 던전 캐릭터 스폰위치 카운트
	int m_dungeon_spawnpos_count;
	// 던전 스테이지 캐릭터 스폰위치
	Vector3 m_dungeon_stage_spawn_pos[DUNGEON_SPAWNPOS_MAXCOUNT];
	// 던전 스테이지 캐릭터 스폰위치 카운트
	int m_dungeon_stage_spawnpos_count;
public:
	DungeonSpawnData();
	~DungeonSpawnData();

	void SetDungeonSpawnPos(Vector3* _v, int _count);
	void SetDungeonSpawnCount(int _count) { m_dungeon_spawnpos_count = _count; }

	void SetDungeonStageSpawnPos(Vector3* _v, int _count);
	void SetDungeonStageSpawnCount(int _count) { m_dungeon_stage_spawnpos_count = _count; }

	// 접근지정자
	const Vector3* Dungeon_Spawn_PosData();
	const Vector3* Dungeon_Stage_Spawn_PosData();
};


class MonsterSpawnData {
private:
	// 몬스터 스폰위치
	Vector3 m_monster_spawn_pos[MONSTER_SPAWNPOS_MAXCOUNT];
	// 몬스터 스폰위치 카운트
	int m_dungeon_spawnpos_count;
public:
	MonsterSpawnData();
	~MonsterSpawnData();

	void SetMonsterSpawnPos(Vector3* _v, int _count);
	void SetMonsterSpawnCount(int _count) { m_dungeon_spawnpos_count = _count; }

	// 접근지정자
	const Vector3* Monster_Spawn_PosData();
};
#endif