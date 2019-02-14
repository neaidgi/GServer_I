#ifndef _GAMEDATAMANAGER_H_
#define _GAMEDATAMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "SpawnData.h"
#include "CharacterData.h"

class GameDataManager
{
private:
	static GameDataManager* Instance;

	GameDataManager();
	~GameDataManager();

	SpawnData* spawndata;
	CharacterData * characterdata;

	bool Player_SpawnPos_Load();
	bool Init_Spawn_Data();			// 스폰위치 초기화(DB에서 스폰위치 읽어옴)
	bool Init_Character_Data();		// 캐릭터 설계도 초기화
public:
	static void CreateInstance();
	static GameDataManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_SpawnPos_Vector(Vector3 * _pos);
	void Character_Origin_Data(int _jobcode, Character* _job);
};

#endif
