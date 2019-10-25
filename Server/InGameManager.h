#ifndef _INGAME_MANAGER_H_
#define _INGAME_MANAGER_H_

#include "Global.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"
#include "UserManager.h"
#include "DBManager.h"
#include "CharacterVerification.h"
#include "CriticalSection.h"
#include "ChannelSystem.h"
#include "GameDataManager.h"
#include "ThreadManager.h"
#include "RandomNumberManager.h"

/*
	[InGameManager]
	
	CreateInstance() - �Ŵ��� ����
	GetInstance() - �Ŵ��� ����������
	DestroyInstance() - �Ŵ��� ����
	
	InitializeManager() - �ʱ�ȭ
	EndManager() - ������
*/

// ����ȭ�� ����ϱ� ���� CMultiThreadSync ��ӹ���
class InGameManager : public CMultiThreadSync<InGameManager>
{
private:
	static InGameManager* Instance;

	InGameManager();
	~InGameManager();

	CharacterVerification* m_verification;
	ChannelSystem*	channelsystem;
	PartyManager* m_partysystem;

	// **Pack �Լ�** //
	// �ٸ� ���� ��ġ���� ��û. (��������,�ڵ�,�����ڵ�,�г���,��ġ,ȸ��)
	void User_Pack_OtherUserPosData(User * _user);
	// ������ ���� ���� ��ŷ. (�ڵ�,�����ڵ�,�г���,��ġ,ȸ��)
	void User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize);
	// �̵� ��û 
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);
	//  ȸ�� ����.  (�ڵ�,ȸ����(vector3))
	void User_Pack_Rotation(User*_user, char* _data, int& _datasize);
	// ä�� ���� ����.(ä�ι�ȣ,ä��������....x6)
	void User_Pack_ChannelInfo(User* _user);
	// ä�� �̵� ��� ��ŷ. (��������,ä�ι�ȣ)
	bool User_Pack_ChannelChangeResult(User* _user, char* _buf, int _choice, int& _datasize, int& _oldchannelnum);
	// ���� ������ ä�θ������ ��ŷ(ä�ι�ȣ)
	void User_Pack_Current_ChannelInfo(User* _user, char* _buf, int& _datasize);
	// ��Ƽ ��� ��ŷ. (���)
	void User_Pack_Party_Result(User * _user, bool _result, char* _data, int& _datasize);
	// ��Ƽ ��� �� �ڵ� ��ŷ. (���,�����̸� �ڵ� ���и� ��)
	void User_Pack_Party_Result_Code(User* _user, bool _result, char* _code, char* _data, int& _datasize);
	// ���� ����� ���� ���� ��ŷ(������ǥ)
	void User_Pack_Dungeon_SpawnData(User * _user,int _count, char* _data, int& _datasize);

	// ���� ����� ä������ ��ŷ. ä�ι�ȣ

	// ���� �̵����� ��ŷ(���ͼ���,�����ڵ�,���͹�ȣ,������ǥ)
	void User_Pack_Monster_MoveInfo(User* _user, int _code, int _num, char*& _data, int& _datasize, UINT64& _sendprotocol);
	// ���� �ǰݰ�� ���� ��ŷ(����or����,�����ڵ�,���͹�ȣ,�������ط�,�׾�����)
	void User_Pack_MonsterAttack_Result(User* _user, bool _result, int _monstercode, int _monsternum, int _damage, bool _isdie);
	// ���� �ǰݰ�� ������ŷ(��������,������,������ false)
	void User_Pack_Under_Attack_Result(User* _user, bool _result, int _damage, bool _state);
	// ���� Ÿ�� ������ŷ(�����ڵ�,���͹�ȣ,�ɸ����ڵ�)
	void User_Pack_Nearset_Character(User* _user, int _monstercode, int _monsternum, Character* _character, char*& _data, int& _datasize, UINT64& _sendprotocol);

	// **UnPack �Լ�**
	// ä�� �̵� ��û ���� (ä�ι�ȣ)
	void User_UnPack_ChannelChange(User* _user, char* _buf, int& _channelnum);
	// ĳ���� �ڵ� ���� (�ڵ�)
	void User_Unpack_UserCode(User* _user, char* _buf, char* _code);
	// �ʴ� ��û ���� (���,�ڵ�,��Ƽ��ȣ)
	void User_Unpack_PartyRoom_Invite_Result(User* _user, char*_buf, bool& _result, char* _code, int& _partyroomnum);
	// ���� �̵� ���� ����(�����ڵ�,���͹�ȣ,��ǥ)
	void User_Unpack_Monster_Move(User* _user, char* _buf, int& _code, int& _num);
	// ���� �̵� ���� ����(�����ڵ�,���͹�ȣ,��ǥ,����)
	void User_Unpack_BossMonster_Move(User* _user, char* _buf, int& _code, int& _num);
	// ������ Ư�� ���͸� �����ߴٴ� ��Ŷ(���� �ڵ�,���� ��ȣ, ���ݹ�ȣ, ������ ���⺤��)
	void User_Unpack_User_Successfully_Attack_The_Monster(User* _user, char* _buf, int& _monstercode, int& _monsternum, int& _attacknum, Vector3& _dir);
	// ������ ���������ߴ��� ����(��������)
	void User_Unpack_Attack_Info(User* _user, char* _buf, int& _attacknum);
	// ���Ͱ� ���������ߴ��� ����(��������)
	void User_Unpack_Monster_Successfully_Attack_The_User(User* _user, char* _buf, int& _monstercode, int& _monsternum, int& _attacknum, Vector3& _dir);
	

	// ** �ٸ��������� �� ���� Pack ** //
	// �̵��̳� ȸ�� ���� ��ŷ (�ڵ�,����3)
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize);
	// ���� �ڵ� ��ŷ. (���� �ش� ������ �ڵ�)
	void User_Pack_UserCode(User* _user, char* _data, int& _datasize);
	// ��Ƽ �ʴ� ��ŷ(�ڵ�,�г���,��Ƽ��ȣ)
	void User_Pack_Party_InviteToOther(User* _user, char* _data, int& _datasize);
	// ��Ƽ�� �����ִ� �������� ��ŷ(��Ƽ��ȣ,��������,�ڵ�,�����ڵ�,�г���,Hp,Mp,��������)
	void User_Pack_Party_CharacterInfoToOther(User* _user, char* _data, int& _datasize);
	// �����ŷ (bool)
	void User_Pack_Party_InviteResultToOther(User* _user, char* _data, int& _datasize, bool _result);
	// ��Ƽ �������� ��ŷ (��������)
	void User_Pack_Party_Protocol(User* _user, char* _data, int& _datasize);
	// ��Ƽ ���� ���� ���� ���� ��ŷ. (Ư�� ������ �ڵ�)
	void User_Pack_Party_KickInfo(User* _user, char* _data, int& _datasize, char*_code);
	// ��Ƽ ���� �ٲ� ���� ���� ��Ŷ. (�ڵ�, �ڵ�)
	void User_Pack_PartyRoom_Leader_Delegate(User* _user, char* _data, int& _datasize, char* _code);
	// ���� ����� ������ġ. (��Ƽ����,�����ڵ�,��ǥ)
	void User_Pack_Party_Dungeon_SpawnData(User* _user,char* _data,int& _datasize);
	// ���� �������� ����� ������ġ. (��Ƽ����,�����ڵ�,��ǥ)
	void User_Pack_Party_Dungeon_Stage_SpawnData(User* _user, UINT64 _p, char* _data, int& _datasize);
	// ���� �������� ����� ���� ����.
	void User_Pack_Dungeon_Monster_SpawnInfo(User* _user, char* _data, int& _datasize);
	// ���� �������� ����� ���� ����.(�����ڵ�,���ͼ���,��������ǥ)
	void User_Pack_Dungeon_Monster_SpawnInfo(User* _user, char* _data, int& _datasize, int _code, int _num, Vector3 _pos);
	// �������� ��ŷ(�����ڵ�,��������)
	void User_Pack_AttackNum_Info(User* _user, char* _data, int& _datasize, int _attacknum);
	// �������� ��ŷ(�����ڵ�,���͹�ȣ,���ݹ�ȣ)
	void User_Pack_Monster_Attack_Info(User* _user, char* _data, int& _datasize,int _code, int _num, int _attackcode);
	// ���� �ǰݰ��. �ٸ��������� ������ŷ(������,�׾��ٴ��ǹ�)
	void User_Pack_MonsterAttackToOher_Result(User* _user, char* _data, int& _datasize, int _monstercode, int _monsternum, int _damage, bool _isdie);
	// ���� �ǰݰ��. �ٸ��������� ������ŷ(ĳ�����ڵ�,������,�׾��ٴ��ǹ�)
	void User_Pack_User_UnderAttackToOher_Result(User* _user, char* _data, int& _datasize,  int _damage, bool _isdie);


	// ** ���� ĳ���� ���� ** //
	// ���� ĳ���� DB�� ����
	void User_CurCharacter_Save(User* _user);
	// �ش� ������ ��Ƽ�� �����ִ���
	bool User_IsParty(char* _code);
	// ���� ���ݽ� �ǰ� ����
	bool User_Attack_Result(User* _user, char* _buf);

	// **Send �Լ�** //
	// �ٸ� ���������� ������ send�Լ� ����(enum������ Ÿ���� ���ؼ� ������)
	void User_Send_ToOther(User* _user, UINT64 _p, SEND_TYPE _type, char* _data, int _datasize, int _channelnum, char* _code);
	// ä�ο� �����ִ� ���������� ����
	void User_Send_In_The_Channel(User* _user, UINT64 _p, char* _data, int& _datasize);
	// Ư�� ä�ο� �����ִ� ���������� ����(ä���̵��Ҷ� ���)
	void User_Send_In_a_Particular_Channel(User* _user, UINT64 _p, int _channelnum, char* _data, int& _datasize);
	// �ٸ� ���� �ΰ��ӿ��� ���� ���� ä�ο� ����(��Ƽ�� ����)
	void User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(User* _user, UINT64 _p, int _channelnum, char* _data, int& _datasize);
	// Ư�� ����(code) ��Ƽ �ʴ� ����
	void User_Send_Party_InviteToOther(User* _user, UINT64 _p, char* _data, int& _datasize, char* _code);
	// ��Ƽ������ ����
	void User_Send_Party_ToOther(User* _user, UINT64 _p, char* _data, int& _datasize);
	// ��Ƽ������ ����(�ش� ���� ����)
	void User_Send_Party(User* _user, UINT64 _p, char* _data, int& _datasize);


	// �ٸ� ���� �ΰ��ӿ��� ���� ���� ����
	void User_Send_LeaveInfoToOther(User* _user, UINT64 _p, char* _data, int& _datasize);
	// ������ ������ ä�ο� �����ִ� ���������� ����
	void User_Send_Party_Enter_Dungeon(User* _user);
	// ������ �������� ä�ο� �����ִ� ���������� ����
	void User_Send_Party_Leave_Dungeon(User* _user);


	//  **���� ����** //
	// ���� ���ݽ� �ǰ� ����
	bool User_Under_Attack(User* _user, char* _buf);
	// �������� ��� �� �������� ����
	void User_Dungeon_Stage_Rise(User* _user);
	// ���� �ð� �ʱ�ȭ
	void User_PartyRoom_Monster_Time_ReSet(User* _user, int _code, int _num);
	// �ش� ������ ��Ƽ�� ���� �ð��� ���ص� �ð��� �����°�.
	bool User_PartyRoom_Monster_TimeOver_Check(User* _user, int _code, int _num);
	// �̸��ʹ� ���������ΰ�
	bool Is_BossMonster(User* _user, int _code);
	// ���� ���� ��������
	bool GetMonsterInfo(User* _user, int _code, int _num, MonsterInfo*& _info);
	// ���� ����� ������ �����ΰ�
	bool GetNearestChracter(User* _user, int _monstercode, int _monsternum, float& _length, Character*& _character);
	// ���� �̵� ���� �� ���� �Ǵ�
	bool Monster_Req_Move(User* _user, char* _buf);
	// ���� ���� �̵� ���� �� ���� �Ǵ�
	bool BossMonster_Req_Move(User* _user, char* _buf);

	// ������ �Լ�(���� ����)
	static DWORD WINAPI MonsterSpawnTimerProcess(LPVOID _user);

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
	// �������� ���� �غ� (��Ƽ��)
	void User_Stage_Ready(User* _user);

	// ��Ƽ�� ����
	PartyRoom* GetPartyRoomSearch(User* _user);
	
};

#endif
