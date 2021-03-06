#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "Global.h"
#include "DBManager.h"
#include "UserManager.h"
#include "GameDataManager.h"
#include "MsgManager.h"
#include "InGameManager.h"
#include "UUIDManager.h"
/*
	[CharacterManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

// 동기화를 사용하기 위해 CMultiThreadSync 상속받음
class CharacterManager : public CMultiThreadSync<CharacterManager>
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
	bool DeleateCharacter(User* _user, char* _buf);										// 캐릭터 삭제
	bool Character_SlotCount_Calculation(User* _user, int _index, int& _slotcount);		// 캐릭터 슬롯이 뒤에 몇개있는지 확인용
	bool Character_SlotPull(User* _user, int _slotnum, int _index);						// 캐릭터 슬롯 당기기(삭제된슬롯번호,당겨야할)
	bool Character_Save(User* _user);													// 캐릭터 저장
public:
	static void CreateInstance();
	static CharacterManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_Slot_Send(User* _user);					// 현재 사용X

	Character* CharacterSelect(User* _user, SlotData*& _slotdata, int _index); // 게임 시작시 실제 캐릭터 조립

	RESULT Character_Management_Process(User* _user);		// 캐릭터 생성창
};

#endif