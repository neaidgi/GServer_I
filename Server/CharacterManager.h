#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "Global.h"
#include "DBManager.h"
#include "User.h"

class CharacterManager 
{
private:
	static CharacterManager* Instance;
	// ���� 3�� list�� ����

	struct SlotData{
		int code;
		char* jobname;
		char* nick;
		int level;

		SlotData() {
			jobname = nullptr;
			nick = nullptr;
		}
		~SlotData() {
			if(jobname != nullptr)
				delete jobname;
			if (nick != nullptr)
				delete nick;
		}
	};

	CharacterManager();
	~CharacterManager();

	bool GetCharacter_Slot(User* _user, int _index, SlotData* _slot);
public:
	static void CreateInstance();
	static CharacterManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Character_Slot_Send(User* _user);

	RESULT Character_Init_Choice(User* _user);
	RESULT Character_Management_Process(User* _user);
	RESULT Character_EnterGame_Process(User* _user);

};

#endif