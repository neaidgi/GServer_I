#include"PartySystem.h"

// Class PartyRoom

PartyRoom::PartyRoom(int _partyroomnum)
{
	m_monster_control = new MonsterControl();
	partyroom_num = _partyroomnum;
	m_dungeon_stage_num = DEFAULT_STAGE;
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
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
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);

		target->SetEnterDungeon();
	}
}

// ���� ����
void PartyRoom::DungeonLeave_PartyRoom()
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);

		target->SetLeaveDungeon();
	}
}

// ���� �˻� �ʱ�ȭ
void PartyRoom::StartSearchMonsterinfo()
{
	m_monster_control->StartSearchMonsterinfo();
}

// ���� �˻�
bool PartyRoom::SearchMonsterinfo(MonsterInfo *& _monsterinfo)
{
	return m_monster_control->SearchMonsterinfo(_monsterinfo);
}

// �������� ���
void PartyRoom::RiseStage()
{
	switch (m_dungeon_stage_num)
	{
	case DEFAULT_STAGE:
		m_dungeon_stage_num = DUNGEON_STAGE_NORMAL_1;
		break;
	case DUNGEON_STAGE_NORMAL_1:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_1;
		break;
	case DUNGEON_STAGE_BOSS_1:
		m_dungeon_stage_num = DUNGEON_STAGE_NORMAL_2;
		break;
	case DUNGEON_STAGE_NORMAL_2:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_2;
		break;
	case DUNGEON_STAGE_BOSS_2:
		m_dungeon_stage_num = DUNGEON_STAGE_NORMAL_3;
		break;
	case DUNGEON_STAGE_NORMAL_3:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_3;
		break;
	case DUNGEON_STAGE_BOSS_3:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_4;
		break;
	default:
		break;
	}
}

// ���������� �´� ���� ���� ����
void PartyRoom::SetDungeonMonsterinfo()
{
	// ���������� �´� ���� ������ �����ϱ����� �����Ͱ� ��������� �����Ѵ�.
	if (m_monster_control->GetMonsterList_Empty() == false)
	{
		m_monster_control->ResetMonsterInfo();
	}

	switch (m_dungeon_stage_num)
	{
	case DUNGEON_STAGE_NORMAL_1:
		m_monster_control->SetFirstStage_NormalMonster();
		break;
	case DUNGEON_STAGE_BOSS_1:
		break;
	case DUNGEON_STAGE_NORMAL_2:
		break;
	case DUNGEON_STAGE_BOSS_2:
		break;
	case DUNGEON_STAGE_NORMAL_3:
		break;
	case DUNGEON_STAGE_BOSS_3:
		break;
	case DUNGEON_STAGE_BOSS_4:
		break;
	default:
		break;
	}
}

// ���� �ð� �ʱ�ȭ
void PartyRoom::Start_MonsterTimer(int _code, int _num)
{
	m_monster_control->InitMonsterTime(_code, _num);
}

// ���� �ð� �������� Ȯ��
bool PartyRoom::End_MonsterTimer(int _code, int _num)
{
	return m_monster_control->Is_End_MonsterTime(_code, _num);
}

// ���� �ڵ� ��ȯ
int PartyRoom::GetMonsterCode(int _count)
{
	return m_monster_control->GetMonsterCode_vector(_count);
}

// ���� ���� ��û
bool PartyRoom::GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo *& _monsterinfo)
{
	return m_monster_control->GetMonsterinfo(_monster_code, _monster_num, _monsterinfo);
}

// ���� ����(�����ڵ�,���͹�ȣ) - ���Ӱ� �����Ҷ�
void PartyRoom::SetMonsterinfo(int _monster_code, int _monster_num)
{
	m_monster_control->SetMonsterinfo(_monster_code, _monster_num);
}

// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,����ȸ����) - �⺻�������� �����Ҷ�
void PartyRoom::SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos)
{
	m_monster_control->SetMonsterinfo(_monster_code, _monster_num, _pos);
}

// ���� ���� ����
bool PartyRoom::RemoveMonsterInfo(int _monster_code, int _monster_num)
{
	return m_monster_control->RemoveMonsterInfo(_monster_code,_monster_num);
}

// ���� ü�� ����
bool PartyRoom::Monster_HP_Down(int _monster_code, int _monster_num, int _damage)
{
	return m_monster_control->Monster_HP_Down(_monster_code,_monster_num,_damage);
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

	for (std::list<PartyRoom*>::iterator save = m_partyroom_list.begin(); save != m_partyroom_list.end(); save++)
	{
		target = (*save);
		if (target->GetPartyRoomNum() == _partyroomnum)
		{
			return target;
		}
	}
}
