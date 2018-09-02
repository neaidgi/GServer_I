#ifndef _DBMANAGER_H_
#define _DBMANAGER_H_

#pragma comment(lib, "libmysql.lib")

#include "Global.h"
#include "User.h"
#include <mysql.h>

class DBManger
{
private:
	static DBManger* Instance;
	MYSQL*	mysql;

	DBManger();
	~DBManger();
public:
	static void CreateInstance();
	static DBManger* GetInstance();
	static void DestroyInstance();

	bool InitializeDB();
	void EndManager();

	bool SelectDB(char* _dbname);
	bool Login_CheckID(char* _id);
	bool Login_reqJoin(char* _id, char* _pw, char* _nick);
	bool Login_reqLogin(char* _id, char* _pw);
	bool Login_reqLeave(char* _id);

	bool Character_CharacterSlotAdd(char* _id, int _index, int _code, char* _jobname, char* _nick, int _level);
	bool Character_reqCharacterInfo(Character* _character_out, int _code);
	bool Character_reqCharacterSlot(char* _id, int _index,  int* _code, char* _jobname, char* _nick, int* _level);
	
	bool Character_reqCharacterDelete(char* _id, int _index);
	bool Character_reqCharacterCheckName(char* _nick);
};

#endif
