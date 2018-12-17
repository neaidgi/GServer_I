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
	bool Login_JoinCharacterSlot(char* _id);
	bool Login_Req_Join(char* _id, char* _pw, char* _nick);
	bool Login_Req_Login(char* _id, char* _pw);
	bool Login_Req_Leave(char* _id);

	bool Character_CharacterSlotAdd(const char* _id, int _index, int _origincode,const char* _jobname, char* _nick, int _level,int _code);
	bool Character_Req_CharacterInfo(Character* _character_out[]);
	bool Character_Req_CharacterSlot(const char* _id, int _index, int& _origincode, char * _jobname, char * _nick, int& _level, int& _code);
	
	bool Character_Req_CharacterDelete(const char* _id, int _index);
	bool Character_Req_CharacterCheckName(const char* _nick);
	bool Character_Req_CharacterPos(int _code, Vector3& _pos);
	bool Charactor_CharacterPosAdd(int _code);

	bool Charactor_Req_CharacterSpawnPos(Vector3 * _pos, int& _count);
};

#endif
