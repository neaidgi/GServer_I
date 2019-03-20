#ifndef _INGAME_MANAGER_H_
#define _INGAME_MANAGER_H_

#include "Global.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"
#include "UserManager.h"
#include "DBManager.h"
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
	void User_Pack_OtherUserPosData(User * _user);																	// 다른 유저 위치정보 요청
	void User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize);
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);			// 이동 요청 
	bool User_Pack_MoveStart(User * _user, char * _buf, int & _datasize, char* _relesedata, int& _rdatasize);		// 이동 시작
	void User_Pack_Rotation(User*_user, char* _data, int& _datasize);												// 
	void User_Pack_ChannelInfo(User* _user);																		// 채널 정보 전송
	bool User_Pack_ChannelChangeResult(User* _user,char* _buf,int _choice,int& _datasize,int& _oldchannelnum);													// 채널 이동 결과

	// **UnPack 함수**																												
	void User_Unpack_CharacterInfo(User* _user, char* _buf);														// 캐릭터 정보
	void User_UnPack_ChannelChange(User* _user, char* _buf, int& _channelnum);										// 채널 이동 요청

	// 이동 정보 다른유저에게 줄 정보
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize);				  						// 이동이나 회전 정보 Vecotr3
	void User_Pack_Leave_InGame(User* _user, char* _data, int& _datasize);											// 유저코드 (특정유저가 게임에서 나가면)
	// 현재 캐릭터 DB에 저장
	void User_CurCharacter_Save(User* _user);

	// **Send 함수**
	void User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);		  // 다른 유저 이동정보 전송
	void User_Send_LeaveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);		  // 다른 유저 인게임에서 떠난 정보 전송
	void User_Send_Channel_LeaveInfoToOther(User* _user, PROTOCOL _p,int _channelnum ,char* _data, int& _datasize);		  // 다른 유저 인게임에서 떠난 정보 전송
public:
	static void CreateInstance();
	static InGameManager* GetInstance();
	static void DestroyInstance();

	bool MangerInitialize();
	void EndManager();

	RESULT	InGame_Init_Packet(User* _user);		// 인게임 요청 처리		 
	bool User_Enter_Channel(User* _user);
	bool User_Enter_Channel(User* _user, int _channelnum);
	void User_Leave_Channel(User* _user);
	bool User_Leave_Channel(User* _user, int _channelnum);

	void User_LeaveInDun_Channel(User* _user);
	void User_EnterInDun_Channel(User* _user);
};

#endif
