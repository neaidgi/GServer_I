#include"PartySystem.h"

PartySystem::PartySystem()
{

}

PartySystem::~PartySystem()
{
	PartyRoom* target = nullptr;

	for (std::list<PartyRoom*>::iterator i = PartyRoomList.begin(); i != PartyRoomList.end(); ++i)
	{
		target = (*i);
		delete target;
	}
}

void PartySystem::Initialize()
{

}

PartyRoom * PartySystem::addPartyroom(User * _user)
{
	PartyRoom* partyroom = nullptr;

	if (_user->isIngame())
	{

	}


	return partyroom;
}

bool PartySystem::Party_Create(User * _user, char* _data, int& _size)
{
	char* ptr = _data;
	int datasize = 0;
	
	PartyRoom* partyroom = nullptr;

	partyroom = new PartyRoom(UUIDManager::GetInstance()->UUID_PartyRoomNum());

	partyroom->PartyRoomAddUser(_user);

	PartyRoomList.push_back(partyroom);

	// 만드는 중

	return true;
}

PartyRoom * PartySystem::PartyRoomSearch(int _partyroomnum)
{
	PartyRoom* target = nullptr;

	for (save = PartyRoomList.begin(); save != PartyRoomList.end(); save++)
	{
		target = (*save);
		if (target->GetPartyRoomNum() == _partyroomnum)
		{
			return target;
		}
	}
	return NULL;
}
