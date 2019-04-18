#ifndef _GAMEDATAMANAGER_H_
#define _GAMEDATAMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "SpawnData.h"
#include "CharacterData.h"
#include "MonsterData.h"

class GameDataManager
{
private:
	static GameDataManager* Instance;

	GameDataManager();
	~GameDataManager();

	SpawnData* spawndata;
	DungeonSpawnData* m_dungeon_spawndata;
	MonsterSpawnData* m_monster_spawndata;
	CharacterData * characterdata;
	MonsterData* m_monsterdata;


	bool Player_SpawnPos_Load();
	bool Init_Spawn_Data();			// 스폰위치 초기화(DB에서 스폰위치 읽어옴)
	bool Init_Character_Data();		// 캐릭터 설계도 초기화
	bool Init_Monster_Data();		// 몬스터 정보 초기화
	bool Dungeon_SpawnPos_Load();	// 던전 스폰위치 저장
	bool Init_Dungeon_Spawn_Data();	// 던전 스폰위치 초기화
	bool Monster_SpawnPos_Load();	// 
	bool Init_Monster_Spawn_Data();	// 스폰위치 초기화(DB에서 스폰위치 읽어옴)
public:
	static void CreateInstance();
	static GameDataManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_SpawnPos_Vector(Vector3 * _pos);
	void Dungeon_SpawnPos_Vecotr(Vector3* _pos);
	void Dungeon_Monster_SpawnPos_Vector(Vector3* _pos);
	void Dungeon_Stage_SpawnPos_Vecotr(Vector3* _pos);

	void GameDataManager::Character_Origin_Data(int _jobcode, const Character*& _job);
	void Monster_Origin_Data(int _monstercode, const Monster*& _monster);
};

#endif
