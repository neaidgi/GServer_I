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

	// **Pack �Լ�**
	void User_Pack_OtherUserPosData(User * _user);																	// �ٸ� ���� ��ġ���� ��û
	void User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize);
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);			// �̵� ��û 
	bool User_Pack_MoveStart(User * _user, char * _buf, int & _datasize, char* _relesedata, int& _rdatasize);		// �̵� ����
	void User_Pack_Rotation(User*_user, char* _data, int& _datasize);												// 
	void User_Pack_ChannelInfo(User* _user);																		// ä�� ���� ����
	bool User_Pack_ChannelChangeResult(User* _user,char* _buf,int _choice,int& _datasize,int& _oldchannelnum);													// ä�� �̵� ���

	// **UnPack �Լ�**																												
	void User_Unpack_CharacterInfo(User* _user, char* _buf);														// ĳ���� ����
	void User_UnPack_ChannelChange(User* _user, char* _buf, int& _channelnum);										// ä�� �̵� ��û

	// �̵� ���� �ٸ��������� �� ����
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize);				  						// �̵��̳� ȸ�� ���� Vecotr3
	void User_Pack_Leave_InGame(User* _user, char* _data, int& _datasize);											// �����ڵ� (Ư�������� ���ӿ��� ������)
	// ���� ĳ���� DB�� ����
	void User_CurCharacter_Save(User* _user);

	// **Send �Լ�**
	void User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);		  // �ٸ� ���� �̵����� ����
	void User_Send_LeaveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);		  // �ٸ� ���� �ΰ��ӿ��� ���� ���� ����
	void User_Send_Channel_LeaveInfoToOther(User* _user, PROTOCOL _p,int _channelnum ,char* _data, int& _datasize);		  // �ٸ� ���� �ΰ��ӿ��� ���� ���� ����
public:
	static void CreateInstance();
	static InGameManager* GetInstance();
	static void DestroyInstance();

	bool MangerInitialize();
	void EndManager();

	RESULT	InGame_Init_Packet(User* _user);		// �ΰ��� ��û ó��		 
	bool User_Enter_Channel(User* _user);
	bool User_Enter_Channel(User* _user, int _channelnum);
	void User_Leave_Channel(User* _user);
	bool User_Leave_Channel(User* _user, int _channelnum);

	void User_LeaveInDun_Channel(User* _user);
	void User_EnterInDun_Channel(User* _user);
};

#endif
