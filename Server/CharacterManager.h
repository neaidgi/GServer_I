#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "Global.h"
#include "DBManager.h"
#include "UserManager.h"
#include "GameDataManager.h"
#include "MsgManager.h"
#include "InGameManager.h"

class CharacterManager 
{
private:
	static CharacterManager* Instance;											  
																				  
	CharacterManager();															  
	~CharacterManager();														  

	bool GetCharacter_Slot(User* _user, int _index, SlotData* _slot);					// 캐릭터 슬롯 가져오기
	bool GetCharacter_SlotCount(User* _user, int& _count);								// 캐릭터 슬롯 개수
	bool NickOverlapCheck(User* _user, char* _nick);									// 캐릭터 닉네임 중복체크
	bool CreateCharacter(User* _user, char* _buf);										// 캐릭터 생성
	bool InitEnterGame(User* _user, char* _buf);										// 게임 접속 초기화
	//void CharacterMove(User* _user, char* _buf, int& _datasize);						// 캐릭터 이동 
	//void CharacterInfo_toOther(User* _user, char* _data, int _datasize);				// 캐릭터 정보 다른유저에게 전송
	bool DeleateCharacter(User* _user, char* _buf);										// 캐릭터 삭제
	bool Character_SlotCount_Calculation(User* _user, int _index, int& _slotcount);		// 캐릭터 슬롯이 뒤에 몇개있는지 확인용
	bool Character_SlotPull(User* _user, int _slotnum, int _index);						// 캐릭터 슬롯 당기기(삭제된슬롯번호,당겨야할)

public:
	static void CreateInstance();
	static CharacterManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_Slot_Send(User* _user);					// 현재 사용X

	Character* CharacterSelect(User* _user, SlotData*& _slotdata, int _index);

	RESULT Character_Init_Choice(User* _user);				// 캐릭터 초기화면
	RESULT Character_Management_Process(User* _user);		// 캐릭터 생성창
	//RESULT Character_EnterGame_Process(User* _user);		// 인게임 
};

#endif