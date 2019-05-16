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

	// ID 중복체크
	bool Login_CheckID(char* _id);
	// 회원가입 
	bool Login_Req_Join(char* _id, char* _pw);
	// 로그인
	bool Login_Req_Login(char* _id, char* _pw);
	// 회원탈퇴
	bool Login_Req_Leave(char* _id);

	// 슬롯 개수 요청
	bool Character_Req_CharacterSlotCount(const char * _id, int& _count);
	// 실제 생성한 유저 캐릭터 저장
	bool Character_CharacterSlotAdd(const char* _id, char* _code, int  _jobcode, const char* _jobname, char* _nick, int _level, int _num);
	// 캐릭터 설계도 요청
	bool Character_Req_CharacterInfo(Character* _character_out[]);
	// 캐릭터 슬롯 요청 (번호 받아서 해당 레코드 가져옴)
	bool Character_Req_CharacterSlot(const char* _id, int _index, int& _jobcode, char * _jobname, char * _nick, int& _level, char* _code);
	// 몬스터 설계도 요청
	bool Character_Req_MonsterInfo(Monster* _monster_out[]);

	// 캐릭터 삭제
	bool Character_Req_CharacterDelete(const char* _id, int _index);
	// 캐릭터 닉네임 중복확인
	bool Character_Req_CharacterCheckName(const char* _nick);
	bool Character_Req_CharacterName(const char* _id, int _index, char* _nick);
	// 캐릭터 위치 요청
	bool Character_Req_CharacterPos(char* _code, Vector3& _pos);
	// 캐릭터 위치 추가
	bool Charactor_CharacterPosAdd(int _code);
	// 삭제한 유저캐릭터 슬롯 뒤에 슬롯이 몇개있는지
	bool Character_Req_SlotCount(const char* _id, int _slotnum, int& _index);
	// 슬롯 당기기
	bool Character_Slot_Pull(const char* _id, int _afterslot, int _beforeslot);
	// 유저 캐릭터 저장하기
	bool Character_Save(const char* _id, Character* _character);

	// 캐릭터 스폰위치 요청
	bool Charactor_Req_CharacterSpawnPos(Vector3 * _pos, int& _count);
	// 던전 캐릭터 스폰위치 요청
	bool Charactor_Req_DungeonSpawnPos(Vector3 * _pos, int& _count);
	// 던전 스테이지 스폰위치 요청
	bool Charactor_Req_DungeonStageSpawnPos(Vector3 * _pos, int& _count);
	// 몬스터 스폰위치 요청
	bool Charactor_Req_MonsterSpawnPos(Vector3 * _pos, int& _count);
};

#endif
