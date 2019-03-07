#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "User.h"
#include <list>

// 파티시스템

struct PartyRoom
{
	int partyroom_num;		// 파티 번호
	int partyroom_usernum;	// 파티 유저 몇명있는지
	User* partyuser[4];		// 유저 배열

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
	// 파티생성
	// 파티참여
	// 파티탈퇴
	// 파티추방
	// 파티원들에게 정보보내기(코드, 체력, 마나)

};


#endif