#include"PartySystem.h"

PartyManager* PartyManager::Instance = nullptr;

PartyManager::PartyManager()
{

}

PartyManager::~PartyManager()
{
	PartyRoom* target = nullptr;

	for (std::list<PartyRoom*>::iterator i = m_partyroom_list.begin(); i != m_partyroom_list.end(); ++i)
	{
		target = (*i);
		delete target;
	}
}

void PartyManager::CreateInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}

	Instance = new PartyManager();
}

PartyManager * PartyManager::GetInstance()
{
	return Instance;
}

void PartyManager::DestroyInstance()
{
	delete Instance;
}

bool PartyManager::InitializeManager()
{
	return false;
}

void PartyManager::EndManager()
{
}

void PartyManager::InitializePartySystem()
{

}

// 리스트 시작부 세팅
void PartyManager::StartSearchPartyRoom()
{
	save = m_partyroom_list.begin();
}

// 유저 추가
bool PartyManager::Partyroom_add_User(User * _user,int _partyroomnum)
{
	PartyRoom* partyroom = nullptr;
	if (_user->isIngame() == true && _user->isParty() == false)
	{
		partyroom = GetPartyRoomSearch(_partyroomnum);

		if (partyroom->GetPartRoomUserNum() >= PARTYMAXUSER && partyroom == nullptr)
		{
			// 파티가 가득찾거나 해당하는 파티방이없으면 실패
			return false;
		}
		// 유저 추가
		partyroom->PartyRoomAddUser(_user);
	}
	return true;
}

// 파티만들기
bool PartyManager::Party_Create(User * _user)
{
	PartyRoom* partyroom = nullptr;

	partyroom = new PartyRoom(UUIDManager::GetInstance()->UUID_PartyRoomNum());

	partyroom->PartyRoomAddUser(_user);
	partyroom->AppointPartyLeader(_user);

	m_partyroom_list.push_back(partyroom);

	return true;
}

// 파티 초대 가능한지
bool PartyManager::Party_Invite(User * _user, User* _targetuser)
{
	PartyRoom* partyroom = nullptr;
	partyroom = GetPartyRoomSearch(_user->GetPartyNum());
	
	if (partyroom == nullptr && _targetuser->isParty() == false)
	{
		// 해당하는 파티가 없으면 실패
		return false;
	}

	return true;
}

// 파티 삭제
bool PartyManager::Party_Remove(User * _user)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;

	if (_user->isPartyLeader() == false)
	{
		// 파티 리더가 아니면 실패
		return false;
	}

	target = GetPartyRoomSearch(_user->GetPartyNum());
	if (target != nullptr)
	{
		target->StartSearchPartyRoom();
		while (target->SearchPartyRoom(user))
		{
			target->PartyRoomRemoveUser(user);
		}

		m_partyroom_list.remove(target);
		delete target;
		target = nullptr;

		return true;
	}

	// 삭제 실패
	return false;
}

// 파티탈퇴
bool PartyManager::Party_User_Leave(User * _user)
{
	PartyRoom* target = nullptr;

	target = GetPartyRoomSearch(_user->GetPartyNum());

	return target->PartyRoomRemoveUser(_user);
}

// 파티 강퇴
bool PartyManager::Party_Kick_User(User * _user, char * _code)
{
	PartyRoom* target = nullptr;

	if (_user->isPartyLeader() == false)
	{
		return false;
	}

	target = GetPartyRoomSearch(_user->GetPartyNum());

	return target->PartyRoomRemoveUser(_code);
}

// 파티장 위임
bool PartyManager::Party_Leader_Delegate(User* _user, char* _code)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;
	// 요청한 유저가 파티리더가 아니면 탈출
	if (_user->isPartyLeader() == false)
	{
		return false;
	}
	target = GetPartyRoomSearch(_user->GetPartyNum());
	// 파티장 해임 실패하면 탈출
	if (target->DismissPartyLeader(_user) == false)
	{
		return false;
	}
	// 파티장 임명 실패하면 전 유저 파티 리더 시키고 탈출
	if (target->AppointPartyLeader(_code) == false)
	{
		target->AppointPartyLeader(_user);
		return false;
	}
	
	return true;
}

// 파티원들이 스테이지에 입장할 준비가 되었는가
bool PartyManager::Party_Is_Members_Ready(User * _user)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;
	bool flag = true;

	target = GetPartyRoomSearch(_user->GetPartyNum());

	// 파티가 있으면
	if (target != nullptr)
	{
		target->StartSearchPartyRoom();
		while (target->SearchPartyRoom(user))
		{
			if (user->GetiStageReady() == false)
			{
				flag = false;
			}
		}
	}

	return flag;
}

// 유저 숫자
int PartyManager::GetPartyRoomUserNum(int _partyroomnum)
{
	PartyRoom* target = nullptr;
	char msg[BUFSIZE];

	target = GetPartyRoomSearch(_partyroomnum);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "~GetPartyRoomUserNum :: 유저 숫자[%d] ", target->GetPartRoomUserNum());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	return target->GetPartRoomUserNum();
}

// 파티방 찾기(번호)
PartyRoom * PartyManager::GetPartyRoomSearch(int _partyroomnum)
{
	PartyRoom* target = nullptr;

	for (std::list<PartyRoom*>::iterator save = m_partyroom_list.begin(); save != m_partyroom_list.end(); save++)
	{
		target = (*save);
		if (target->GetPartyRoomNum() == _partyroomnum)
		{
			return target;
		}
	}
}

/* send 담당 */
// 파티원들에게 send
void PartyManager::Party_Send_ToMembers(User * _user, UINT64 _p, char * _data, int & _datasize)
{
	User* user = nullptr;
	PartyRoom* partyroom = nullptr;

	if (_user->isParty())
	{
		// 파티방 객체 가져오기
		partyroom = GetPartyRoomSearch(_user->GetPartyNum());

		// 검색 초기화
		partyroom->StartSearchPartyRoom();
		// 검색 시작
		while (partyroom->SearchPartyRoom(user))
		{
			if (user->isIngame() && user->getSocket() != _user->getSocket())
			{
				user->Quepack(_p, _data, _datasize);
				if (user->isSending() == false)
				{
					if (user->TakeOutSendPacket())
					{
						user->IOCP_SendMsg();
					}
				}
				// 메세지
				//char msg[BUFSIZE];
				//memset(msg, 0, sizeof(msg));
				//sprintf(msg, "Party_Send_ToMembers :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				//	_p, _datasize, user->GetSendQueueSize());
				//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
			}
		}
	}
}

// 특정 파티원에게 send
void PartyManager::Party_Send_ToOther(User * _user, UINT64 _p, char * _data, int & _datasize)
{
}
