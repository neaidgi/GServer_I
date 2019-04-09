#include"PartySystem.h"

// Class PartyRoom

PartyRoom::PartyRoom(int _partyroomnum)
{
	m_monster_control = nullptr;
	partyroom_num = _partyroomnum;
}

PartyRoom::~PartyRoom()
{
	User* target = nullptr;
	char msg[BUFSIZE];
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		m_partyuser.remove(target);

		// 메세지
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "~PartyRoom :: 파티방 소멸자 ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	if (m_monster_control != nullptr)
	{
		delete m_monster_control;
	}
}

// 초기화
void PartyRoom::InitPartyRoom()
{
	if (m_monster_control == nullptr)
	{
		m_monster_control = new MonsterControl();
	}
}

// 마무리
void PartyRoom::DestroyPartyRoom()
{

}

// 검색 초기화
void PartyRoom::StartSearchPartyRoom()
{
	save = m_partyuser.begin();
}

// 파티원(유저) 전달
bool PartyRoom::SearchPartyRoom(User *& _user)
{
	// 유저 하나씩 꺼내서 save에 넣음
	if (save != m_partyuser.end())
	{
		_user = *save;
		++save;
		return true;
	}
	else
	{
		return false;
	}
}

// 파티방 번호가 무엇인가
int PartyRoom::GetPartyRoomNum()
{
	return partyroom_num;
}

// 현재 파티원 인원수
int PartyRoom::GetPartRoomUserNum()
{
	return m_partyuser.size();
}

// 유저 추가
void PartyRoom::PartyRoomAddUser(User* _user)
{
	if (m_partyuser.size() < PARTYMAXUSER)
	{
		m_partyuser.push_back(_user);
		_user->SetPartyNum(partyroom_num);
	}
}

// 유저 삭제
bool PartyRoom::PartyRoomRemoveUser(User* _user)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (target == _user)
		{
			m_partyuser.remove(target);
			_user->ResetPartyInfo();
			return true;
		}
	}
}

// 유저 삭제(코드)
bool PartyRoom::PartyRoomRemoveUser(char * _code)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (strcmp(target->GetCurCharacter()->GetCharacter_Code(), _code) == 0)
		{
			target->ResetPartyInfo();
			m_partyuser.remove(target);
			return true;
		}
	}
}

// 파티장 임명
bool PartyRoom::AppointPartyLeader(User* _user)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (target == _user)
		{
			_user->SetPartyLeader(true);
			return true;
		}
	}
	return false;
}

// 파티장 임명(코드)
bool PartyRoom::AppointPartyLeader(char * _code)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (strcmp(target->GetCurCharacter()->GetCharacter_Code(), _code) == 0)
		{
			target->SetPartyLeader(true);
			return true;
		}
	}
	return false;
}

// 파티장 해임
bool PartyRoom::DismissPartyLeader(User* _user)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// 그 유저가있고 파티장이면
		if (target == _user && target->isPartyLeader() == true)
		{
			_user->SetPartyLeader(false);
			return true;
		}
	}
	return false;
}

// 파티장 해임(코드)
bool PartyRoom::DismissPartyLeader(char * _code)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// 그 유저가있고 파티장이면
		if (target->GetCurCharacter()->GetCharacter_Code() == _code && target->isPartyLeader() == true)
		{
			target->SetPartyLeader(false);
			return true;
		}
	}
	return false;
}

// 파티장이 누구인가
User* PartyRoom::WhopartyLeader()
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// 그 유저가있고 파티장이면
		if (target->isPartyLeader() == true)
		{
			return target;
		}
	}
}

// 던전 입장
void PartyRoom::DungeonEnter_PartyRoom()
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);

		target->SetEnterDungeon();
	}
}

// 던전 퇴장
void PartyRoom::DungeonLeave_PartyRoom()
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);

		target->SetLeaveDungeon();
	}
}



// Class PartySystem
PartySystem::PartySystem()
{

}

PartySystem::~PartySystem()
{
	PartyRoom* target = nullptr;

	for (std::list<PartyRoom*>::iterator i = m_partyroom_list.begin(); i != m_partyroom_list.end(); ++i)
	{
		target = (*i);
		delete target;
	}
}

void PartySystem::InitializePartySystem()
{

}

// 리스트 시작부 세팅
void PartySystem::StartSearchPartyRoom()
{
	save = m_partyroom_list.begin();
}

// 유저 추가
bool PartySystem::Partyroom_add_User(User * _user,int _partyroomnum)
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
bool PartySystem::Party_Create(User * _user)
{
	PartyRoom* partyroom = nullptr;

	partyroom = new PartyRoom(UUIDManager::GetInstance()->UUID_PartyRoomNum());

	partyroom->PartyRoomAddUser(_user);
	partyroom->AppointPartyLeader(_user);

	m_partyroom_list.push_back(partyroom);

	return true;
}

// 파티 초대 가능한지
bool PartySystem::Party_Invite(User * _user, User* _targetuser)
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
bool PartySystem::Party_Remove(User * _user)
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
		while (1)
		{
			if (target->SearchPartyRoom(user) == true)
			{
				user->ResetPartyInfo();
			}
			else
			{
				break;
			}
		}

		m_partyroom_list.remove(target);
		return true;
	}

	// 삭제 실패
	return false;
}

// 파티탈퇴
bool PartySystem::Party_User_Leave(User * _user)
{
	PartyRoom* target = nullptr;

	target = GetPartyRoomSearch(_user->GetPartyNum());

	return target->PartyRoomRemoveUser(_user);
}

// 파티 강퇴
bool PartySystem::Party_Kick_User(User * _user, char * _code)
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
bool PartySystem::Party_Leader_Delegate(User* _user, char* _code)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;

	if (_user->isPartyLeader() == false)
	{
		return false;
	}
	target = GetPartyRoomSearch(_user->GetPartyNum());

	if (target->DismissPartyLeader(_user) == false)
	{
		return false;
	}

	if (target->AppointPartyLeader(_code) == false)
	{
		target->AppointPartyLeader(_user);
		return false;
	}
	
	return true;
}

// 유저 숫자
int PartySystem::GetPartyRoomUserNum(int _partyroomnum)
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
PartyRoom * PartySystem::GetPartyRoomSearch(int _partyroomnum)
{
	PartyRoom* target = nullptr;

	for (save = m_partyroom_list.begin(); save != m_partyroom_list.end(); save++)
	{
		target = (*save);
		if (target->GetPartyRoomNum() == _partyroomnum)
		{
			return target;
		}
	}
}
