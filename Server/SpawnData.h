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

	bool Player_SpawnPos_Load();
public:
	SpawnData();
	~SpawnData();
	
	// 접근지정자
	const Vector3* Character_Spawn_PosData();

	// 스폰위치 초기화(DB에서 스폰위치 읽어옴)
	bool Init_Spawn_Data();
};

#endif