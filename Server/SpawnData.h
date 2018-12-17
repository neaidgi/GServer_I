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

	bool Player_SpawnPos_Load();
public:
	SpawnData();
	~SpawnData();
	
	// ����������
	const Vector3* Character_Spawn_PosData();

	// ������ġ �ʱ�ȭ(DB���� ������ġ �о��)
	bool Init_Spawn_Data();
};

#endif