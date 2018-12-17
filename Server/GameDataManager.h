#ifndef _GAMEDATAMANAGER_H_
#define _GAMEDATAMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "SpawnData.h"

class GameDataManager
{
private:
	static GameDataManager* Instance;

	GameDataManager();
	~GameDataManager();

	SpawnData* spawndata;
public:
	static void CreateInstance();
	static GameDataManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_SpawnPos_Vector(Vector3 * _pos);
};

#endif
