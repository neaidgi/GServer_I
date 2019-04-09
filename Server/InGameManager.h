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
	PartySystem* m_partysystem;

	// **Pack �Լ�** //
	// �ٸ� ���� ��ġ���� ��û. ��������,�ڵ�,�����ڵ�,�г���,��ġ,ȸ��
	void User_Pack_OtherUserPosData(User * _user);
	// ������ ���� ���� ��ŷ. �ڵ�,�����ڵ�,�г���,��ġ,ȸ��
	void User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize);
	// �̵� ��û 
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);
	// �̵� ����
	bool User_Pack_MoveStart(User * _user, char * _buf, int & _datasize, char* _relesedata, int& _rdatasize);
	//  ȸ�� ����. �ڵ�,ȸ����(vector3)
	void User_Pack_Rotation(User*_user, char* _data, int& _datasize);
	// ä�� ���� ����. ä�ι�ȣ,ä��������....x6
	void User_Pack_ChannelInfo(User* _user);
	// ä�� �̵� ��� ��ŷ. ��������,ä�ι�ȣ
	bool User_Pack_ChannelChangeResult(User* _user, char* _buf, int _choice, int& _datasize, int& _oldchannelnum);
	// ���� ������ ä�θ������ ��ŷ
	void User_Pack_Current_ChannelInfo(User* _user, char* _buf, int& _datasize);
	// Ư���� �������� �ʴ� ��û ��ŷ(��Ƽ���ȣ,�����ڵ�,�����г���)

	// ��Ƽ �ʴ� ��� ��ŷ. ���
	void User_Pack_Party_Invite_Result(User * _user, bool _result, char* _data, int& _datasize);
	// ��Ƽ ���� ��� ��ŷ.���,(�����̸� �ڵ� ���и� ��)
	void User_Pack_Party_Kick_Result(User* _user, bool _result, char* _code, char* _data, int& _datasize);
	// ��Ƽ Ż�� ��� ��ŷ.���
	void User_Pack_Party_Leave_Result(User* _user, bool _result, char* _data, int& _datasize);
	// ��Ƽ ���� ���� ��� ��ŷ.���,(�����̸� �ڵ� ���и� ��)
	void User_Pack_Party_Leader_Delegate_Result(User* _user, bool _result, char* _code, char* _data, int& _datasize);
	
	// ���� ����� ä������ ��ŷ. ä�ι�ȣ

	// **UnPack �Լ�**
	// ĳ���� ����
	void User_Unpack_CharacterInfo(User* _user, char* _buf);
	// ä�� �̵� ��û
	void User_UnPack_ChannelChange(User* _user, char* _buf, int& _channelnum);
	// �ʴ� ��û ���� (���� ĳ���� �ڵ�)
	void User_Unpack_PartyRoom_Invite(User* _user, char* _buf, char* _code);
	// �ʴ� ��û ���
	void User_Unpack_PartyRoom_Invite_Result(User* _user, char*_buf, bool& _result, char* _code, int& _partyroomnum);
	// ��Ƽ�� ���� ��û
	void User_Unpack_PartyRoom_Kick(User* _user, char* _buf, char* _code);
	// ��Ƽ ���� ����
	void User_Unpack_PartyRoom_Leader_Delegate(User* _user, char* _buf, char* _code);

	// ** �ٸ��������� �� ���� Pack ** //
	// �̵��̳� ȸ�� ���� Vecotr3. 
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize);
	// �����ڵ� (Ư�������� ���ӿ��� ������)
	void User_Pack_Leave_InGame(User* _user, char* _data, int& _datasize);
	// ��Ƽ �ʴ�
	void User_Pack_Party_InviteToOther(User* _user, char* _data, int& _datasize);
	// ��Ƽ ���� ĳ���� ���� pack
	void User_Pack_Party_CharacterInfoToOther(User* _user, char* _data, int& _datasize);
	// ��Ƽ �ʴ��ߴµ� �濡 ��������. ���
	void User_Pack_Party_InviteResultToOther(User* _user, char* _data, int& _datasize);
	// ��Ƽ ������ߴٴ� ���� ��ŷ(��� �ƹ��͵�����). ��������
	void User_Pack_Party_Kicked(User* _user, char* _data, int& _datasize);
	// ��Ƽ ���� ���� ���� ���� ��ŷ. �ڵ�
	void User_Pack_Party_KickInfo(User* _user, char* _data, int& _datasize, char*_code);
	// ��Ƽ�� �������� ��Ƽ�������� �����ٴ� ���� ��ŷ. �������ݸ�
	void User_Pack_PartyRoom_Leader_Leave(User* _user, char* _data, int& _datasize);
	// ��Ƽ Ż���� ���� ���� ��ŷ
	//void User_Pack_Party_LeaveInfo(User* _user, char* _data, int& _datasize);
	// ��Ƽ ���� �ٲ� ���� ���� ��Ŷ. �ڵ�,�ڵ�
	void User_Pack_PartyRoom_Leader_Delegate(User* _user, char* _data, int& _datasize, char* _code);

	// ���� ĳ���� DB�� ����
	void User_CurCharacter_Save(User* _user);

	// �ش� ������ ��Ƽ�� �����ִ���
	bool User_IsParty(char* _code);

	// **Send �Լ�** //
	// �ٸ� ���� �̵����� ����
	void User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);
	// �ٸ� ���� �ΰ��ӿ��� ���� ���� ����
	void User_Send_LeaveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);
	// �ٸ� ���� �ΰ��ӿ��� ���� ���� ä�ο� ����
	void User_Send_Channel_LeaveInfoToOther(User* _user, PROTOCOL _p, int _channelnum, char* _data, int& _datasize);
	// �ٸ� ���� �ΰ��ӿ��� ���� ���� ä�ο� ����(��Ƽ�� ����)
	void User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(User* _user, PROTOCOL _p, int _channelnum, char* _data, int& _datasize);
	// Ư�� ����(code) ��Ƽ �ʴ� ����
	void User_Send_Party_InviteToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize, char* _code);
	// ��Ƽ������ ����
	void User_Send_Party_ToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);
	// ������ ������ ä�ο� �����ִ� ���������� ����
	void User_Send_Party_Eneter_Dungeon(User* _user, PROTOCOL _p);
	// ������ �������� ä�ο� �����ִ� ���������� ����
	void User_Send_Party_Leave_Dungeon(User* _user, PROTOCOL _p);
public:
	static void CreateInstance();
	static InGameManager* GetInstance();
	static void DestroyInstance();

	bool MangerInitialize();
	void EndManager();
	// �ΰ��� ��û ó��
	RESULT	InGame_Init_Packet(User* _user);
	// �ΰ��� ���¿��� ���������� ��ó��
	bool User_InGame_Compulsion_Exit(User* _user);
	// ä�� ����
	bool User_Enter_Channel(User* _user);
	// Ư�� ä�� ����
	bool User_Enter_Channel(User* _user, int _channelnum);
	// ä�� ������
	void User_Leave_Channel(User* _user);
	// Ư�� ä�� ������
	bool User_Leave_Channel(User* _user, int _channelnum);


	// ��Ƽ�� ����
	bool User_Create_PartyRoom(User* _user);
	// ��Ƽ�� ����
	bool User_Remove_PartyRoom(User* _user);
	// ��Ƽ�� ����(�ڵ�)
	bool User_Remove_PartyRoom(char* _code);
	// ��Ƽ �ʴ�
	//bool User_Party_Invite(User* _user,char* _code);	
	// ��Ƽ�� ���� �߰�(�����ϴ������ڵ�,��Ƽ���ȣ)
	bool User_Party_AddUser(User* _user, int _partyroomnum);
	// �ش� ���� ��Ƽ�� �ο���
	int User_PartyRoom_UserNum(User* _user);
	//  ��Ƽ�濡 ȥ���ΰ�
	bool User_PartyRoom_Alone(int _partyroomnum);
	// ��Ƽ ���� ����
	bool User_PartyRoom_User_Kick(User* _user, char* _code);
	// ��Ƽ ���� Ż��
	bool User_PartRoom_User_Leave(User* _user);
	// ��Ƽ�� ����
	bool User_PartyRoom_Leader_Delegate(User* _user, char* _code);
	// ���� ä�� ����
	void User_EnterInDun_Channel(User* _user);
	// ���� ä�� ������
	void User_LeaveInDun_Channel(User* _user);
};

#endif
