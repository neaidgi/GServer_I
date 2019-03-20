#ifndef _DBMANAGER_H_
#define _DBMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "Character.h"
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

	bool Login_CheckID(char* _id);
	bool Login_Req_Join(char* _id, char* _pw);
	bool Login_Req_Login(char* _id, char* _pw);
	bool Login_Req_Leave(char* _id);

	bool Character_Req_CharacterSlotCount(const char * _id, int& _count);
	bool Character_CharacterSlotAdd(const char* _id, char* _code,int  _jobcode,const char* _jobname, char* _nick, int _level, int _num);
	bool Character_Req_CharacterInfo(Character* _character_out[]);
	bool Character_Req_CharacterSlot(const char* _id, int _index, int& _jobcode, char * _jobname, char * _nick, int& _level, char* _code);
	
	bool Character_Req_CharacterDelete(const char* _id,int _index);
	bool Character_Req_CharacterCheckName(const char* _nick);
	bool Character_Req_CharacterName(const char* _id, int _index, char* _nick);
	bool Character_Req_CharacterPos(char* _code, Vector3& _pos);
	bool Charactor_CharacterPosAdd(int _code);
	bool Character_Req_SlotCount(const char* _id, int _slotnum, int& _index);		// 삭제한 유저캐릭터 슬롯 뒤에 슬롯이 몇개있는지
	bool Character_Slot_Pull(const char* _id, int _afterslot, int _beforeslot);		// 슬롯 당기기
	bool Character_Save(const char* _id, Character* _character);										// 유저 캐릭터 저장하기

	bool Charactor_Req_CharacterSpawnPos(Vector3 * _pos, int& _count);
};

#endif
