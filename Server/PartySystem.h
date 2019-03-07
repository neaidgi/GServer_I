#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "User.h"
#include <list>

// ��Ƽ�ý���

struct PartyRoom
{
	int partyroom_num;		// ��Ƽ ��ȣ
	int partyroom_usernum;	// ��Ƽ ���� ����ִ���
	User* partyuser[4];		// ���� �迭

	PartyRoom() 
	{ 
		partyroom_num = 0; 
		partyroom_usernum = 0;
		memset(partyuser, 0, sizeof(partyuser)); 
	}
	~PartyRoom() 
	{

	}
};

class PartySystem
{
private:
	std::list<PartyRoom*> PartyRoomList;
	std::list<PartyRoom*>::iterator save;

public:
	PartySystem();
	~PartySystem();

	void Initialize();
	// ��Ƽ����
	// ��Ƽ����
	// ��ƼŻ��
	// ��Ƽ�߹�
	// ��Ƽ���鿡�� ����������(�ڵ�, ü��, ����)

};


#endif