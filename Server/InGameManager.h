#ifndef _INGAME_MANAGER_H_
#define _INGAME_MANAGER_H_

#include "Global.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"
#include "UserManager.h"
#include "CharacterVerification.h"

class InGameManager
{
private:
	static InGameManager* Instance;

	InGameManager();
	~InGameManager();

	CharacterVerification* verification;

	void User_Req_OtherUserPosData(User * _user);
	bool User_Req_Move(User * _user, char* _buf, int& _datasize);
	void User_Send_MoveInfotoOther(User* _user, char* _data, int& _datasize);
public:
	static void CreateInstance();
	static InGameManager* GetInstance();
	static void DestroyInstance();

	bool MangerInitialize();
	void EndManager();

	RESULT	InGame_Init_Packet(User* _user);		// 인게임 요청 처리		 
};

#endif
