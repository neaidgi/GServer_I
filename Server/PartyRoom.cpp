#include "PartyRoom.h"

// Class PartyRoom

PartyRoom::PartyRoom()
{
	partyroom_num = 0;
	m_monster_control = nullptr;
	m_stage_info = nullptr;
}

PartyRoom::PartyRoom(int _partyroomnum)
{
	m_monster_control = new MonsterControl();
	m_stage_info = new StageInfo();
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
	const StageInfo* stageinfo = nullptr;
	int code[STAGE_MONSTER_NUM];
	int num[STAGE_MONSTER_NUM];

	switch (m_stage_info->GetStage_Num())
	{
	case DEFAULT_STAGE:
		GameDataManager::GetInstance()->Stage_Origin_Data(DUNGEON_STAGE_NORMAL_1, stageinfo);
		m_stage_info->SetStage_Num(DUNGEON_STAGE_NORMAL_1);
		stageinfo->GetStage_NormalMonster(code);
		stageinfo->GetStage_NormalMonster_Num(num);
		m_stage_info->SetStage_NormalMonster_Code(code);
		m_stage_info->SetStage_NormalMonster_Num(num);
		m_stage_info->SetStage_Normal();
		break;
	case DUNGEON_STAGE_NORMAL_1:
		GameDataManager::GetInstance()->Stage_Origin_Data(DUNGEON_STAGE_NORMAL_1, stageinfo);
		m_stage_info->SetStage_Num(DUNGEON_STAGE_BOSS_1);
		m_stage_info->SetStage_BossMonster_Code(stageinfo->GetStage_BossMonster());
		m_stage_info->SetStage_Boss();
		break;
	case DUNGEON_STAGE_BOSS_1:
		GameDataManager::GetInstance()->Stage_Origin_Data(DUNGEON_STAGE_NORMAL_2, stageinfo);
		m_stage_info->SetStage_Num(DUNGEON_STAGE_NORMAL_2);
		stageinfo->GetStage_NormalMonster(code);
		stageinfo->GetStage_NormalMonster_Num(num);
		m_stage_info->SetStage_NormalMonster_Code(code);
		m_stage_info->SetStage_NormalMonster_Num(num);
		m_stage_info->SetStage_Normal();
		break;
	case DUNGEON_STAGE_NORMAL_2:
		GameDataManager::GetInstance()->Stage_Origin_Data(DUNGEON_STAGE_NORMAL_2, stageinfo);
		m_stage_info->SetStage_Num(DUNGEON_STAGE_BOSS_2);
		m_stage_info->SetStage_BossMonster_Code(stageinfo->GetStage_BossMonster());
		m_stage_info->SetStage_Boss();
		break;
	case DUNGEON_STAGE_BOSS_2:
		GameDataManager::GetInstance()->Stage_Origin_Data(DUNGEON_STAGE_NORMAL_3, stageinfo);
		m_stage_info->SetStage_Num(DUNGEON_STAGE_NORMAL_3);
		stageinfo->GetStage_NormalMonster(code);
		stageinfo->GetStage_NormalMonster_Num(num);
		m_stage_info->SetStage_NormalMonster_Code(code);
		m_stage_info->SetStage_NormalMonster_Num(num);
		m_stage_info->SetStage_Normal();
		break;
	case DUNGEON_STAGE_NORMAL_3:

		break;
	case DUNGEON_STAGE_BOSS_3:
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

	// �Ϲ� ����������
	if (m_stage_info->GetStage_Is_Boss_Stage() == false)
	{
		int code[2];
		int num[2];

		m_stage_info->GetStage_NormalMonster(code);
		m_stage_info->GetStage_NormalMonster_Num(num);
		m_monster_control->Stage_SetMonster(code, num);
	}
	else // ���� ���������̸�
	{
		int bosscode = 0;
		int bossnum = 0;

		bosscode = m_stage_info->GetStage_BossMonster();
		m_monster_control->Stage_SetBossMonster(bosscode, bossnum);
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
int PartyRoom::GetMonsterCode(int _code)
{
	return m_monster_control->GetMonsterCode_vector(_code);
}

// Ư�� ��Ŀ ��� ����
int PartyRoom::GetMonsterNum(int _code)
{
	return m_monster_control->GetMonsterNum(_code);
}

bool PartyRoom::Is_BossMonster(int _code)
{
	return m_monster_control->Is_BossMonster(_code);
}

void PartyRoom::SetBossMonsterAttackInfo(int _attacknum, int _attacktime)
{
	m_monster_control->SetBossMonsterAttackInfo(_attacknum, _attacktime);
}

bool PartyRoom::Is_BossMonster_What_Attack(MonsterInfo * _monster, float _length, int & _attackcode)
{
	return m_monster_control->Is_BossMonster_What_Attack(_monster, _length, _attackcode);
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

// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ) - �⺻�������� �����Ҷ�
void PartyRoom::SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos)
{
	m_monster_control->SetMonsterinfo(_monster_code, _monster_num, _pos);
}

// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,���͹���) - �⺻�������� �����Ҷ�
void PartyRoom::SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _dir)
{
	m_monster_control->SetMonsterinfo(_monster_code, _monster_num, _pos, _dir);
}

// ���� ���� ����
bool PartyRoom::RemoveMonsterInfo(int _monster_code, int _monster_num)
{
	return m_monster_control->RemoveMonsterInfo(_monster_code, _monster_num);
}

// ���� ü�� ����
bool PartyRoom::Monster_HP_Down(int _monster_code, int _monster_num, int _damage)
{
	return m_monster_control->Monster_HP_Down(_monster_code, _monster_num, _damage);
}
