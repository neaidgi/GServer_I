#ifndef _GAMEDATAMANAGER_H_
#define _GAMEDATAMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "SpawnData.h"
#include "CharacterData.h"
#include "MonsterData.h"
/*
	[GameDataManager]
	
	CreateInstance() - �Ŵ��� ����
	GetInstance() - �Ŵ��� ����������
	DestroyInstance() - �Ŵ��� ����
	
	InitializeManager() - �ʱ�ȭ
	EndManager() - ������
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
	bool Init_Spawn_Data();			// ������ġ �ʱ�ȭ(DB���� ������ġ �о��)
	bool Init_Character_Data();		// ĳ���� ���赵 �ʱ�ȭ
	bool Init_Monster_Data();		// ���� ���� �ʱ�ȭ
	bool Dungeon_SpawnPos_Load();	// ���� ������ġ ����
	bool Init_Dungeon_Spawn_Data();	// ���� ������ġ �ʱ�ȭ
	bool Monster_SpawnPos_Load();	// 
	bool Init_Monster_Spawn_Data();	// ������ġ �ʱ�ȭ(DB���� ������ġ �о��)
	bool Init_Stage_Data();			// �������� ���� �ʱ�ȭ
public:
	static void CreateInstance();
	static GameDataManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// �÷��̾� ������ġ �迭
	void Character_SpawnPos_Vector(Vector3 * _pos);
	// ���� ������ġ �迭
	void Dungeon_SpawnPos_Vecotr(Vector3* _pos);
	// ���� ���� ������ġ �迭
	void Dungeon_Monster_SpawnPos_Vector(Vector3* _pos);
	// ���� �������� ������ġ �迭
	void Dungeon_Stage_SpawnPos_Vecotr(Vector3* _pos);

	// ���������� ���赵 ������
	void GameDataManager::Character_Origin_Data(int _jobcode, const Character*& _job);
	// ���� �ڵ忡 ���� ���赵 ������
	void Monster_Origin_Data(int _monstercode, const Monster*& _monster);
	// �������� ��ȣ ���� ���赵 ������
	void Stage_Origin_Data(int _stage_num, const StageInfo*& _stage);
};

#endif
