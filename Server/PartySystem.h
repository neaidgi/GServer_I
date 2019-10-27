#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "UUIDManager.h"
#include "PartyRoom.h"
#include <list>

#define PARTYMAXUSER 4

// ��Ƽ�ý���
class PartyManager
{
private:
	static PartyManager* Instance;

	std::list<PartyRoom*> m_partyroom_list;
	std::list<PartyRoom*>::iterator save;
public:
	PartyManager();
	~PartyManager();

	static void CreateInstance();
	static PartyManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

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
	
	// ��Ƽ������ ���������� ������ �غ� �Ǿ��°�
	bool Party_Is_Members_Ready(User* _user);

	// �ش� ���� ��Ƽ �ο���
	int GetPartyRoomUserNum(int _partyroomnum);
	// ��Ƽ�� �˻�(��Ƽ��ȣ)
	PartyRoom* GetPartyRoomSearch(int _partyroomnum);
	
	/* send ��� */
	// ��Ƽ���鿡�� send
	void Party_Send_ToMembers(User* _user, UINT64 _p, char * _data, int & _datasize);
	// Ư�� ��Ƽ������ send
	void Party_Send_ToOther(User * _user, UINT64 _p, char * _data, int & _datasize);
};


#endif