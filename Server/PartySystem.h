#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "User.h"
#include "UUIDManager.h"
#include <list>

#define PARTYMAXUSER 4

// 파티방안에 있는 유저정보 구조체
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

// 파티방
class PartyRoom
{
private:
	char partyroom_num;			// 파티 번호
	int partyroom_usernum;		// 파티 유저 몇명있는지

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

	// 파티방 번호가 무엇인가
	int GetPartyRoomNum()
	{
		return partyroom_num;
	}

	// 유저 추가
	void PartyRoomAddUser(User* _user)
	{
		PartyUser* newpartyuser = nullptr;

		if (partyuser.size() <= PARTYMAXUSER)
		{
			newpartyuser = new PartyUser(_user);

			partyuser.push_back(newpartyuser);
		}
	}

	// 유저 삭제
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

	// 파티유저숫자증가
	bool PartyUserNum_Increase()
	{
		if (partyroom_usernum + 1 >= PARTYMAXUSER)
		{
			return false;
		}
		partyroom_usernum++;
		return true;
	}

	// 파티유저숫자감소
	bool PartyUserNum_Reduction()
	{
		if (partyroom_usernum - 1 <= 0)
		{
			return false;
		}
		partyroom_usernum--;
		return true;
	}

	// 파티장 임명
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

	// 파티장 해임
	bool DismissPartyLeader(User* _user)
	{
		PartyUser* target = nullptr;
		for (save = partyuser.begin(); save != partyuser.end(); save++)
		{
			target = (*save);
			// 그 유저가있고 파티장이면
			if (target->user == _user && target->Partyleader == true)
			{
				target->Partyleader = false;
				return true;
			}
		}
		return false;
	}

	// 파티장이 누구인가
	User* WhopartyLeader()
	{
		PartyUser* target = nullptr;
		for (save = partyuser.begin(); save != partyuser.end(); save++)
		{
			target = (*save);
			// 그 유저가있고 파티장이면
			if (target->Partyleader == true)
			{
				return target->user;
			}
		}
		return nullptr;
	}
};

// 파티시스템
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
	bool Party_Create(User* _user, char* _data, int& _size);	// 파티생성
	// 파티참여
	// 파티초대
	// 파티탈퇴
	// 파티추방
	// 파티원들에게 정보보내기(코드, 체력, 마나)
	PartyRoom* PartyRoomSearch(int _partyroomnum);// 파티방 검색(int _partyroomnum)
};


#endif