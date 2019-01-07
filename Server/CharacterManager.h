#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "Global.h"
#include "DBManager.h"
#include "UserManager.h"
#include "GameDataManager.h"

class CharacterManager 
{
private:
	static CharacterManager* Instance;

	CharacterManager();
	~CharacterManager();

	bool GetCharacter_Slot(User* _user, int _index, SlotData* _slot);
	bool NickOverlapCheck(User* _user, char* _nick);
	void CreateCharacter(User* _user, char* _buf);
	void InitEnterGame(User* _user, char* _buf);
	void CharacterMove(User* _user, char* _buf, int& _datasize);
	void CharacterInfo_toOther(User* _user, char* _data, int _datasize);
	int CharacterCode();	// 캐릭터 고유 코드 만들기(임시사용)

public:
	static void CreateInstance();
	static CharacterManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_Slot_Send(User* _user);					// 현재 사용X

	Character* CharacterSelect(User* _user, int _index);

	RESULT Character_Init_Choice(User* _user);				// 캐릭터 초기화면
	RESULT Character_Management_Process(User* _user);		// 캐릭터 생성창
	RESULT Character_EnterGame_Process(User* _user);		// 인게임 
};

#endif