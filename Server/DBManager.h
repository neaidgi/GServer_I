#ifndef _DBMANAGER_H_
#define _DBMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "Character.h"
#include "Monster.h"
#include <mysql.h>

class DBManager
{
private:
	static DBManager* Instance;
	MYSQL*	mysql;

	DBManager();
	~DBManager();
public:
	static void CreateInstance();
	static DBManager* GetInstance();
	static void DestroyInstance();

	bool InitializeDB();
	void EndManager();

	bool SelectDB(char* _dbname);

	// ID �ߺ�üũ
	bool Login_CheckID(char* _id);
	// ȸ������ 
	bool Login_Req_Join(char* _id, char* _pw);
	// �α���
	bool Login_Req_Login(char* _id, char* _pw);
	// ȸ��Ż��
	bool Login_Req_Leave(char* _id);

	// ���� ���� ��û
	bool Character_Req_CharacterSlotCount(const char * _id, int& _count);
	// ���� ������ ���� ĳ���� ����
	bool Character_CharacterSlotAdd(const char* _id, char* _code, int  _jobcode, const char* _jobname, char* _nick, int _level, int _num);
	// ĳ���� ���赵 ��û
	bool Character_Req_CharacterInfo(Character* _character_out[]);
	// ĳ���� ���� ��û (��ȣ �޾Ƽ� �ش� ���ڵ� ������)
	bool Character_Req_CharacterSlot(const char* _id, int _index, int& _jobcode, char * _jobname, char * _nick, int& _level, char* _code);
	// ���� ���赵 ��û
	bool Character_Req_MonsterInfo(Monster* _monster_out[]);

	// ĳ���� ����
	bool Character_Req_CharacterDelete(const char* _id, int _index);
	// ĳ���� �г��� �ߺ�Ȯ��
	bool Character_Req_CharacterCheckName(const char* _nick);
	bool Character_Req_CharacterName(const char* _id, int _index, char* _nick);
	// ĳ���� ��ġ ��û
	bool Character_Req_CharacterPos(char* _code, Vector3& _pos);
	// ĳ���� ��ġ �߰�
	bool Charactor_CharacterPosAdd(int _code);
	// ������ ����ĳ���� ���� �ڿ� ������ ��ִ���
	bool Character_Req_SlotCount(const char* _id, int _slotnum, int& _index);
	// ���� ����
	bool Character_Slot_Pull(const char* _id, int _afterslot, int _beforeslot);
	// ���� ĳ���� �����ϱ�
	bool Character_Save(const char* _id, Character* _character);

	// ĳ���� ������ġ ��û
	bool Charactor_Req_CharacterSpawnPos(Vector3 * _pos, int& _count);
	// ���� ĳ���� ������ġ ��û
	bool Charactor_Req_DungeonSpawnPos(Vector3 * _pos, int& _count);
	// ���� �������� ������ġ ��û
	bool Charactor_Req_DungeonStageSpawnPos(Vector3 * _pos, int& _count);
	// ���� ������ġ ��û
	bool Charactor_Req_MonsterSpawnPos(Vector3 * _pos, int& _count);
};

#endif
