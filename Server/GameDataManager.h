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
	bool Init_Spawn_Data();			// ������ġ �ʱ�ȭ(DB���� ������ġ �о��)
	bool Init_Character_Data();		// ĳ���� ���赵 �ʱ�ȭ
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
