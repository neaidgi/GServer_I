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
	bool Login_reqJoin(char* _id, char* _pw, char* _nick);
	bool Login_reqLogin(char* _id, char* _pw);
	bool Login_reqLeave(char* _id);

	bool Character_CharacterSlotAdd(const char* _id, int _index, int _origincode,const char* _jobname, char* _nick, int _level,int _code);
	bool Character_reqCharacterInfo(Character* _character_out[]);
	bool Character_reqCharacterSlot(const char* _id, int _index, int& _origincode, char * _jobname, char * _nick, int& _level, int& _code);
	
	bool Character_reqCharacterDelete(const char* _id, int _index);
	bool Character_reqCharacterCheckName(const char* _nick);
	bool Character_reqCharacterPos(int _code, Vector3& _pos);
};

#endif
