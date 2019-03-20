#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "User.h"
#include "UUIDManager.h"
#include <list>

#define PARTYMAXUSER 4

// ��Ƽ��ȿ� �ִ� �������� ����ü
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
	char partyroom_num;			// ��Ƽ ��ȣ
	int partyroom_usernum;		// ��Ƽ ���� ����ִ���

	std::list<PartyUser*> partyuser;
	std::list<PartyUser*>::iterator save;

public:

	PartyRoom()
	{
		partyroom_num = 0;
		partyroom_usernum = 0;
	}

	PartyRoom(int _partyroomnum)
	{
		partyroom_num = _partyroomnum;
		partyroom_usernum = 0;
	}

	~PartyRoom()
	{
		PartyUser* target = nullptr;

		for (save = partyuser.begin(); save != partyuser.end(); save++)
		{
			target = (*save);
			delete target;
		}
	}

	// ��Ƽ�� ��ȣ�� �����ΰ�
	int GetPartyRoomNum()
	{
		return partyroom_num;
	}

	// ���� �߰�
	void PartyRoomAddUser(User* _user)
	{
		PartyUser* newpartyuser = nullptr;

		if (partyuser.size() <= PARTYMAXUSER)
		{
			newpartyuser = new PartyUser(_user);

			partyuser.push_back(newpartyuser);
		}
	}

	// ���� ����
	void PartyRoomRemoveUser(User* _user)
	{
		PartyUser* target = nullptr;
		for (save = partyuser.begin(); save != partyuser.end(); save++)
		{
			target = (*save);
			if (target->user == _user)
			{
				partyuser.remove(target);
				delete target;
				target = nullptr;
			}
		}
	}

	// ��Ƽ������������
	bool PartyUserNum_Increase()
	{
		if (partyroom_usernum + 1 >= PARTYMAXUSER)
		{
			return false;
		}
		partyroom_usernum++;
		return true;
	}

	// ��Ƽ�������ڰ���
	bool PartyUserNum_Reduction()
	{
		if (partyroom_usernum - 1 <= 0)
		{
			return false;
		}
		partyroom_usernum--;
		return true;
	}

	// ��Ƽ�� �Ӹ�
	bool AppointPartyLeader(User* _user)
	{
		PartyUser* target = nullptr;
		for (save = partyuser.begin(); save != partyuser.end(); save++)
		{
			target = (*save);
			if (target->user == _user)
			{
				target->Partyleader = true;
				return true;
			}
		}
		return false;
	}

	// ��Ƽ�� ����
	bool DismissPartyLeader(User* _user)
	{
		PartyUser* target = nullptr;
		for (save = partyuser.begin(); save != partyuser.end(); save++)
		{
			target = (*save);
			// �� �������ְ� ��Ƽ���̸�
			if (target->user == _user && target->Partyleader == true)
			{
				target->Partyleader = false;
				return true;
			}
		}
		return false;
	}

	// ��Ƽ���� �����ΰ�
	User* WhopartyLeader()
	{
		PartyUser* target = nullptr;
		for (save = partyuser.begin(); save != partyuser.end(); save++)
		{
			target = (*save);
			// �� �������ְ� ��Ƽ���̸�
			if (target->Partyleader == true)
			{
				return target->user;
			}
		}
		return nullptr;
	}
};

// ��Ƽ�ý���
class PartySystem
{
private:
	std::list<PartyRoom*> PartyRoomList;
	std::list<PartyRoom*>::iterator save;

public:
	PartySystem();
	~PartySystem();

	void Initialize();
	PartyRoom* addPartyroom(User* _user);
	bool Party_Create(User* _user, char* _data, int& _size);	// ��Ƽ����
	// ��Ƽ����
	// ��Ƽ�ʴ�
	// ��ƼŻ��
	// ��Ƽ�߹�
	// ��Ƽ���鿡�� ����������(�ڵ�, ü��, ����)
	PartyRoom* PartyRoomSearch(int _partyroomnum);// ��Ƽ�� �˻�(int _partyroomnum)
};


#endif