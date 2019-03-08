#ifndef _INGAME_MANAGER_H_
#define _INGAME_MANAGER_H_

#include "Global.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"
#include "UserManager.h"
#include "CharacterVerification.h"
#include "CriticalSectionManager.h"
#include "ChannelSystem.h"

class InGameManager
{
private:
	static InGameManager* Instance;

	InGameManager();
	~InGameManager();

	CharacterVerification* verification;
	ChannelSystem*	channelsystem;

	// **Pack 함수**
	void User_Pack_OtherUserPosData(User * _user);											 // 다른 유저 위치정보 요청
	void User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize);
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);							 // 이동 요청 
	bool User_Pack_MoveStart(User * _user, char * _buf, int & _datasize, char* _relesedata, int& _rdatasize);					 // 이동 시작
	void User_Pack_Rotation(User*_user, char* _data, int& _datasize);
	// 이동 정보 다른유저에게 줄 정보
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize);				  

	// **Send 함수**
	void User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);		  // 다른 유저 이동정보 전송
public:
	static void CreateInstance();
	static InGameManager* GetInstance();
	static void DestroyInstance();

	bool MangerInitialize();
	void EndManager();

	RESULT	InGame_Init_Packet(User* _user);		// 인게임 요청 처리		 
	bool User_Enter_Channel(User* _user);
	void User_Leave_Channel(User* _user);

	void User_LeaveInDun_Channel(User* _user);
	void User_EnterInDun_Channel(User* _user);
};

#endif
