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

#endif