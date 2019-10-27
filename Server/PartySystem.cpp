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

// ����Ʈ ���ۺ� ����
void PartyManager::StartSearchPartyRoom()
{
	save = m_partyroom_list.begin();
}

// ���� �߰�
bool PartyManager::Partyroom_add_User(User * _user,int _partyroomnum)
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
bool PartyManager::Party_Create(User * _user)
{
	PartyRoom* partyroom = nullptr;

	partyroom = new PartyRoom(UUIDManager::GetInstance()->UUID_PartyRoomNum());

	partyroom->PartyRoomAddUser(_user);
	partyroom->AppointPartyLeader(_user);

	m_partyroom_list.push_back(partyroom);

	return true;
}

// ��Ƽ �ʴ� ��������
bool PartyManager::Party_Invite(User * _user, User* _targetuser)
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
bool PartyManager::Party_Remove(User * _user)
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
		while (target->SearchPartyRoom(user))
		{
			target->PartyRoomRemoveUser(user);
		}

		m_partyroom_list.remove(target);
		delete target;
		target = nullptr;

		return true;
	}

	// ���� ����
	return false;
}

// ��ƼŻ��
bool PartyManager::Party_User_Leave(User * _user)
{
	PartyRoom* target = nullptr;

	target = GetPartyRoomSearch(_user->GetPartyNum());

	return target->PartyRoomRemoveUser(_user);
}

// ��Ƽ ����
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

// ��Ƽ�� ����
bool PartyManager::Party_Leader_Delegate(User* _user, char* _code)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;
	// ��û�� ������ ��Ƽ������ �ƴϸ� Ż��
	if (_user->isPartyLeader() == false)
	{
		return false;
	}
	target = GetPartyRoomSearch(_user->GetPartyNum());
	// ��Ƽ�� ���� �����ϸ� Ż��
	if (target->DismissPartyLeader(_user) == false)
	{
		return false;
	}
	// ��Ƽ�� �Ӹ� �����ϸ� �� ���� ��Ƽ ���� ��Ű�� Ż��
	if (target->AppointPartyLeader(_code) == false)
	{
		target->AppointPartyLeader(_user);
		return false;
	}
	
	return true;
}

// ��Ƽ������ ���������� ������ �غ� �Ǿ��°�
bool PartyManager::Party_Is_Members_Ready(User * _user)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;
	bool flag = true;

	target = GetPartyRoomSearch(_user->GetPartyNum());

	// ��Ƽ�� ������
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

// ���� ����
int PartyManager::GetPartyRoomUserNum(int _partyroomnum)
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

/* send ��� */
// ��Ƽ���鿡�� send
void PartyManager::Party_Send_ToMembers(User * _user, UINT64 _p, char * _data, int & _datasize)
{
	User* user = nullptr;
	PartyRoom* partyroom = nullptr;

	if (_user->isParty())
	{
		// ��Ƽ�� ��ü ��������
		partyroom = GetPartyRoomSearch(_user->GetPartyNum());

		// �˻� �ʱ�ȭ
		partyroom->StartSearchPartyRoom();
		// �˻� ����
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
				// �޼���
				//char msg[BUFSIZE];
				//memset(msg, 0, sizeof(msg));
				//sprintf(msg, "Party_Send_ToMembers :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				//	_p, _datasize, user->GetSendQueueSize());
				//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
			}
		}
	}
}

// Ư�� ��Ƽ������ send
void PartyManager::Party_Send_ToOther(User * _user, UINT64 _p, char * _data, int & _datasize)
{
}
