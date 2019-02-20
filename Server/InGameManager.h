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

	void User_Pack_OtherUserPosData(User * _user);											 // �ٸ� ���� ��ġ���� ��û
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);							 // �̵� ��û 
	bool User_Pack_MoveStart(User * _user, char * _buf, int & _datasize, char* _relesedata, int& _rdatasize);					 // �̵� ����
	// �̵� ���� �ٸ��������� �� ����
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize, float _dirx, float _diry);				  

	void User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);		  // �ٸ� ���� �̵����� ����
public:
	static void CreateInstance();
	static InGameManager* GetInstance();
	static void DestroyInstance();

	bool MangerInitialize();
	void EndManager();

	RESULT	InGame_Init_Packet(User* _user);		// �ΰ��� ��û ó��		 
};

#endif
