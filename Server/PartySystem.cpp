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

		// �޼���
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "~PartyRoom :: ��Ƽ�� �Ҹ��� ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	if (m_monster_control != nullptr)
	{
		delete m_monster_control;
	}
}

// �ʱ�ȭ
void PartyRoom::InitPartyRoom()
{
	if (m_monster_control == nullptr)
	{
		m_monster_control = new MonsterControl();
	}
}

// ������
void PartyRoom::DestroyPartyRoom()
{

}

// �˻� �ʱ�ȭ
void PartyRoom::StartSearchPartyRoom()
{
	save = m_partyuser.begin();
}

// ��Ƽ��(����) ����
bool PartyRoom::SearchPartyRoom(User *& _user)
{
	// ���� �ϳ��� ������ save�� ����
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

// ��Ƽ�� ��ȣ�� �����ΰ�
int PartyRoom::GetPartyRoomNum()
{
	return partyroom_num;
}

// ���� ��Ƽ�� �ο���
int PartyRoom::GetPartRoomUserNum()
{
	return m_partyuser.size();
}

// ���� �߰�
void PartyRoom::PartyRoomAddUser(User* _user)
{
	if (m_partyuser.size() < PARTYMAXUSER)
	{
		m_partyuser.push_back(_user);
		_user->SetPartyNum(partyroom_num);
	}
}

// ���� ����
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

// ���� ����(�ڵ�)
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

// ��Ƽ�� �Ӹ�
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

// ��Ƽ�� �Ӹ�(�ڵ�)
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

// ��Ƽ�� ����
bool PartyRoom::DismissPartyLeader(User* _user)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// �� �������ְ� ��Ƽ���̸�
		if (target == _user && target->isPartyLeader() == true)
		{
			_user->SetPartyLeader(false);
			return true;
		}
	}
	return false;
}

// ��Ƽ�� ����(�ڵ�)
bool PartyRoom::DismissPartyLeader(char * _code)
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// �� �������ְ� ��Ƽ���̸�
		if (target->GetCurCharacter()->GetCharacter_Code() == _code && target->isPartyLeader() == true)
		{
			target->SetPartyLeader(false);
			return true;
		}
	}
	return false;
}

// ��Ƽ���� �����ΰ�
User* PartyRoom::WhopartyLeader()
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// �� �������ְ� ��Ƽ���̸�
		if (target->isPartyLeader() == true)
		{
			return target;
		}
	}
}

// ���� ����
void PartyRoom::DungeonEnter_PartyRoom()
{
	User* target = nullptr;
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);

		target->SetEnterDungeon();
	}
}

// ���� ����
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

// ����Ʈ ���ۺ� ����
void PartySystem::StartSearchPartyRoom()
{
	save = m_partyroom_list.begin();
}

// ���� �߰�
bool PartySystem::Partyroom_add_User(User * _user,int _partyroomnum)
{
	PartyRoom* partyroom = nullptr;
	if (_user->isIngame() == true && _user->isParty() == false)
	{
		partyroom = GetPartyRoomSearch(_partyroomnum);

		if (partyroom->GetPartRoomUserNum() >= PARTYMAXUSER && partyroom == nullptr)
		{
			// ��Ƽ�� ����ã�ų� �ش��ϴ� ��Ƽ���̾����� ����
			return false;
		}
		// ���� �߰�
		partyroom->PartyRoomAddUser(_user);
	}
	return true;
}

// ��Ƽ�����
bool PartySystem::Party_Create(User * _user)
{
	PartyRoom* partyroom = nullptr;

	partyroom = new PartyRoom(UUIDManager::GetInstance()->UUID_PartyRoomNum());

	partyroom->PartyRoomAddUser(_user);
	partyroom->AppointPartyLeader(_user);

	m_partyroom_list.push_back(partyroom);

	return true;
}

// ��Ƽ �ʴ� ��������
bool PartySystem::Party_Invite(User * _user, User* _targetuser)
{
	PartyRoom* partyroom = nullptr;
	partyroom = GetPartyRoomSearch(_user->GetPartyNum());
	
	if (partyroom == nullptr && _targetuser->isParty() == false)
	{
		// �ش��ϴ� ��Ƽ�� ������ ����
		return false;
	}

	return true;
}

// ��Ƽ ����
bool PartySystem::Party_Remove(User * _user)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;

	if (_user->isPartyLeader() == false)
	{
		// ��Ƽ ������ �ƴϸ� ����
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

	// ���� ����
	return false;
}

// ��ƼŻ��
bool PartySystem::Party_User_Leave(User * _user)
{
	PartyRoom* target = nullptr;

	target = GetPartyRoomSearch(_user->GetPartyNum());

	return target->PartyRoomRemoveUser(_user);
}

// ��Ƽ ����
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

// ��Ƽ�� ����
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

// ���� ����
int PartySystem::GetPartyRoomUserNum(int _partyroomnum)
{
	PartyRoom* target = nullptr;
	char msg[BUFSIZE];

	target = GetPartyRoomSearch(_partyroomnum);

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "~GetPartyRoomUserNum :: ���� ����[%d] ", target->GetPartRoomUserNum());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	return target->GetPartRoomUserNum();
}

// ��Ƽ�� ã��(��ȣ)
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
