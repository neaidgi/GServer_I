#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "User.h"
#include "UUIDManager.h"
#include <list>

#define PARTYMAXUSER 4

// ��Ƽ��ȿ� �ִ� �������� ����ü // ��������� 2019-03-21
struct PartyUser
{
	User* user;
	bool Partyleader;

	PartyUser()
	{
		user = nullptr; Partyleader = false;
	}

	PartyUser(User* _user)
	{
		user = _user; Partyleader = false;
	}

	~PartyUser()
	{
		user = nullptr;
	}
};

// ��Ƽ��
class PartyRoom
{
private:
	// ��Ƽ ��ȣ
	char partyroom_num;			

	// ��Ƽ ���� ����Ʈ
	std::list<User*> m_partyuser;	
	std::list<User*>::iterator save;

public:
	PartyRoom();
	PartyRoom(int _partyroomnum);
	~PartyRoom();

	// �˻� �ʱ�ȭ
	void StartSearchPartyRoom();
	// ��Ƽ��(����) ����
	bool SearchPartyRoom(User*& _user);
	// ��Ƽ�� ��ȣ
	int GetPartyRoomNum();
	// ���� ��Ƽ�� �ο���
	int GetPartRoomUserNum();
	// ���� �߰�
	void PartyRoomAddUser(User* _user);
	// ���� �߰�(�ڵ�)

	// ���� ����
	bool PartyRoomRemoveUser(User* _user);
	// ���� ����(�ڵ�)
	bool PartyRoomRemoveUser(char* _code);
	// ��Ƽ�� �Ӹ�
	bool AppointPartyLeader(User* _user);
	// ��Ƽ�� �Ӹ�
	bool AppointPartyLeader(char* _code);
	// ��Ƽ�� ����
	bool DismissPartyLeader(User* _user);
	// ��Ƽ�� ����
	bool DismissPartyLeader(char* _code);
	// ��Ƽ���� �����ΰ�
	User* WhopartyLeader();
	// ���� ����
	void DungeonEnter_PartyRoom();
	// ���� ����
	void DungeonLeave_PartyRoom();

};

// ��Ƽ�ý���
class PartySystem
{
private:
	std::list<PartyRoom*> m_partyroom_list;
	std::list<PartyRoom*>::iterator save;
public:
	PartySystem();
	~PartySystem();

	void InitializePartySystem();
	// �˻� �ʱ�ȭ
	void StartSearchPartyRoom();

	// ���� �߰�
	bool Partyroom_add_User(User* _user, int _partyroomnum);
	// ��Ƽ����
	bool Party_Create(User* _user);
	// ��Ƽ����

	// ��Ƽ �ʴ� ��������
	bool Party_Invite(User* _user, User* _targetuser);
	// ��Ƽ ����
	bool Party_Remove(User* _user);
	// ��ƼŻ��
	bool Party_User_Leave(User* _user);
	// ��Ƽ ����
	bool Party_Kick_User(User* _user,char* _code);
	// ��Ƽ���鿡�� ����������(�ڵ�, ü��, ����)

	// ��Ƽ�� ����
	bool Party_Leader_Delegate(User* _user, char* _code);

	// �ش� ���� ��Ƽ �ο���
	int GetPartyRoomUserNum(int _partyroomnum);
	// ��Ƽ�� �˻�(��Ƽ��ȣ)
	PartyRoom* GetPartyRoomSearch(int _partyroomnum);
};


#endif