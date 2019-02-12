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

	bool GetCharacter_Slot(User* _user, int _index, SlotData* _slot);			// ĳ���� ���� ��������
	bool NickOverlapCheck(User* _user, char* _nick);							// ĳ���� �г��� �ߺ�üũ
	void CreateCharacter(User* _user, char* _buf);								// ĳ���� ����
	void InitEnterGame(User* _user, char* _buf);								// ���� ���� �ʱ�ȭ
	void CharacterMove(User* _user, char* _buf, int& _datasize);				// ĳ���� �̵� 
	bool CharacterMoveVerificate(User* _user, char* _buf);						// ĳ���� �̵� ����
	void CharacterInfo_toOther(User* _user, char* _data, int _datasize);		// ĳ���� ���� �ٸ��������� ����
	void DeleateCharacter(User* _user, char* _buf);													// ĳ���� ����

public:
	static void CreateInstance();
	static CharacterManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_Slot_Send(User* _user);					// ���� ���X

	Character* CharacterSelect(User* _user, int _index);

	RESULT Character_Init_Choice(User* _user);				// ĳ���� �ʱ�ȭ��
	RESULT Character_Management_Process(User* _user);		// ĳ���� ����â
	RESULT Character_EnterGame_Process(User* _user);		// �ΰ��� 
};

#endif