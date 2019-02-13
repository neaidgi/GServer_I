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

	bool GetCharacter_Slot(User* _user, int _index, SlotData* _slot);			// 캐릭터 슬롯 가져오기
	bool GetCharacter_SlotCount(User* _user, int& _count);						// 캐릭터 슬롯 개수
	bool NickOverlapCheck(User* _user, char* _nick);							// 캐릭터 닉네임 중복체크
	void CreateCharacter(User* _user, char* _buf);								// 캐릭터 생성
	void InitEnterGame(User* _user, char* _buf);								// 게임 접속 초기화
	void CharacterMove(User* _user, char* _buf, int& _datasize);				// 캐릭터 이동 
	bool CharacterMoveVerificate(User* _user, char* _buf);						// 캐릭터 이동 검증
	void CharacterInfo_toOther(User* _user, char* _data, int _datasize);		// 캐릭터 정보 다른유저에게 전송
	void DeleateCharacter(User* _user, char* _buf);													// 캐릭터 삭제

public:
	static void CreateInstance();
	static CharacterManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_Slot_Send(User* _user);					// 현재 사용X

	Character* CharacterSelect(User* _user, SlotData _slotdata, int _index);

	RESULT Character_Init_Choice(User* _user);				// 캐릭터 초기화면
	RESULT Character_Management_Process(User* _user);		// 캐릭터 생성창
	RESULT Character_EnterGame_Process(User* _user);		// 인게임 
};

#endif