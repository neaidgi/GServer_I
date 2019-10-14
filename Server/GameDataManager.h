#ifndef _GAMEDATAMANAGER_H_
#define _GAMEDATAMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "SpawnData.h"
#include "CharacterData.h"
#include "MonsterData.h"
/*
	[GameDataManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

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
	StageData* m_stagedata;


	bool Player_SpawnPos_Load();
	bool Init_Spawn_Data();			// 스폰위치 초기화(DB에서 스폰위치 읽어옴)
	bool Init_Character_Data();		// 캐릭터 설계도 초기화
	bool Init_Monster_Data();		// 몬스터 정보 초기화
	bool Dungeon_SpawnPos_Load();	// 던전 스폰위치 저장
	bool Init_Dungeon_Spawn_Data();	// 던전 스폰위치 초기화
	bool Monster_SpawnPos_Load();	// 
	bool Init_Monster_Spawn_Data();	// 스폰위치 초기화(DB에서 스폰위치 읽어옴)
	bool Init_Stage_Data();			// 스테이지 정보 초기화
public:
	static void CreateInstance();
	static GameDataManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// 플레이어 스폰위치 배열
	void Character_SpawnPos_Vector(Vector3 * _pos);
	// 던전 스폰위치 배열
	void Dungeon_SpawnPos_Vecotr(Vector3* _pos);
	// 던전 몬스터 스폰위치 배열
	void Dungeon_Monster_SpawnPos_Vector(Vector3* _pos);
	// 던전 스테이지 스폰위치 배열
	void Dungeon_Stage_SpawnPos_Vecotr(Vector3* _pos);

	// 직업에따라 설계도 가져옴
	void GameDataManager::Character_Origin_Data(int _jobcode, const Character*& _job);
	// 몬스터 코드에 따라 설계도 가져옴
	void Monster_Origin_Data(int _monstercode, const Monster*& _monster);
	// 스테이지 번호 따라 설계도 가져옴
	void Stage_Origin_Data(int _stage_num, const StageInfo*& _stage);
};

#endif
