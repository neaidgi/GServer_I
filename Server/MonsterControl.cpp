#include "MonsterControl.h"

MonsterInfo::MonsterInfo()
{
	m_monster_time = nullptr;
	m_monster = nullptr;
	m_monster_info_num = -1;
	m_monster_activate = true;

}

MonsterInfo::~MonsterInfo()
{
	if (m_monster_time != nullptr)
	{
		delete m_monster_time;
		m_monster_time = nullptr;
	}

	if (m_monster != nullptr)
	{
		delete m_monster;
		m_monster = nullptr;
	}
}

// ���� Ÿ�̸� �ʱ�ȭ
void MonsterInfo::CreateMonsterTimer()
{
	m_monster_time = new MonsterTime();
}

// ���� Ÿ�̸� �ð� �ʱ�ȭ
void MonsterInfo::ResetMonsterTime()
{
	m_monster_time->Start_Time();
}

// ���� �ð��� ��������
bool MonsterInfo::Is_End_MonsterTime()
{
	double time = m_monster_time->End_Time();

	// �޼���
	//char msg[BUFSIZE];
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, " ���� Ÿ�� [%lf]", time );
	//MsgManager::GetInstance()->DisplayMsg("TIME", msg);

	if (time >= MONSTERTIME)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// �̸��ʹ� ���������ΰ�
bool MonsterInfo::Is_BossMonster()
{
	return m_monster->GetMonster_Is_Boss();
}

// MonsterControl
MonsterControl::MonsterControl()
{
	monster_types_num = 0;
	m_boss_on_the_attack = false;
	m_boss_attack_num = 0;
	m_boss_attack_max_time = 0;
}

MonsterControl::~MonsterControl()
{
	MonsterInfo* target = nullptr;

	for (m_monsterinfo_save = m_monsterinfo_list.begin(); m_monsterinfo_save != m_monsterinfo_list.end(); ++m_monsterinfo_save)
	{
		target = (*m_monsterinfo_save);
		delete target;
	}
}

// �˻� �ʱ�ȭ
void MonsterControl::StartSearchMonsterinfo()
{
	m_monsterinfo_save = m_monsterinfo_list.begin();
}

// �˻�
bool MonsterControl::SearchMonsterinfo(MonsterInfo *& _monsterinfo)
{
	if (m_monsterinfo_save != m_monsterinfo_list.end())
	{
		_monsterinfo = *m_monsterinfo_save;
		++m_monsterinfo_save;
		return true;
	}
	else
	{
		return false;
	}
}

// ���� �ڵ� �߰�
void MonsterControl::AddMonsterCode_vector(int _code)
{
	m_monstercode_vector.push_back(_code);
}

// ���� �ڵ� ��ȯ
int MonsterControl::GetMonsterCode_vector(int _count)
{
	return m_monstercode_vector.at(_count);
}

// ���� ���� ����
Monster * MonsterControl::MonsterSelect(int _monster_code)
{
	Monster* monster = new Monster();
	const Monster* origin_monster = nullptr;
	GameDataManager::GetInstance()->Monster_Origin_Data(_monster_code, origin_monster);

	// ���� �⺻����
	monster->SetMonster_AttackPoint(origin_monster->GetMonster_AttackPoint());
	monster->SetMonster_Code(origin_monster->GetMonster_Code());
	monster->SetMonster_DefensePoint(origin_monster->GetMonster_DefensePoint());
	monster->SetMonster_HP(origin_monster->GetMonster_HP());
	monster->SetMonster_MP(origin_monster->GetMonster_MP());
	monster->SetMonster_Current_HP(origin_monster->GetMonster_HP());
	monster->SetMonster_Current_MP(origin_monster->GetMonster_MP());
	monster->SetMonster_Name(origin_monster->GetMonster_Name());
	monster->SetMonster_Num(origin_monster->GetMonster_Num());
	monster->SetMonster_Speed(origin_monster->GetMonster_Speed());
	monster->SetPosition(origin_monster->GetPosition());
	monster->SetRotation(origin_monster->GetRotation());
	monster->SetScale(origin_monster->GetScale());

	// ��������
	for (int i = 0; i < origin_monster->GetAttackInfoSize(); i++)
	{
		monster->AddAttackInfo(origin_monster->GetAttackInfo().at(i));
	}
	return monster;
}

// ���� Ÿ�̸� �ð��ʱ�ȭ
void MonsterControl::InitMonsterTime(int _code, int _num)
{
	MonsterInfo* monsterinfo = nullptr;
	GetMonsterinfo(_code, _num, monsterinfo);

	monsterinfo->ResetMonsterTime();
}

// ���� �ð��� ��������
bool MonsterControl::Is_End_MonsterTime(int _code, int _num)
{
	MonsterInfo* monsterinfo = nullptr;
	if (GetMonsterinfo(_code, _num, monsterinfo) == false)
	{
		return false;
	}

	return 	monsterinfo->Is_End_MonsterTime();
}

// ���� ���� �˻�
bool MonsterControl::GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo *& _monsterinfo)
{
	MonsterInfo* monsterinfo = nullptr;

	CThreadSync cs;

	StartSearchMonsterinfo();
	while (SearchMonsterinfo(monsterinfo))
	{
		if (monsterinfo->GetMonster()->GetMonster_Code() == _monster_code && monsterinfo->GetMonsterNum() == _monster_num)
		{
			_monsterinfo = monsterinfo;
			return true;
		}
	}
	return false;
}

// ���� ����(�����ڵ�,���͹�ȣ) - ���Ӱ� �����Ҷ�
void MonsterControl::SetMonsterinfo(int _monster_code, int _monster_num)
{
	MonsterInfo* monsterinfo = new MonsterInfo();

	monsterinfo->SetMonster(MonsterSelect(_monster_code));
	monsterinfo->SetMonsterNum(_monster_num);
	monsterinfo->CreateMonsterTimer();
	monsterinfo->ResetMonsterTime();

	m_monsterinfo_list.push_back(monsterinfo);
}

// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ) - �⺻�������� �����Ҷ�
void MonsterControl::SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos)
{
	MonsterInfo* monsterinfo = nullptr;
	GetMonsterinfo(_monster_code, _monster_num, monsterinfo);
	monsterinfo->GetMonster()->SetPosition(_pos);
}

// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,���͹���) - �⺻�������� �����Ҷ�
void MonsterControl::SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _dir)
{
	MonsterInfo* monsterinfo = nullptr;
	GetMonsterinfo(_monster_code, _monster_num, monsterinfo);
	monsterinfo->GetMonster()->SetPosition(_pos);
	monsterinfo->GetMonster()->SetRotation(_dir);
}

// ���� ���� ����
bool MonsterControl::RemoveMonsterInfo(int _monster_code, int _monster_num)
{
	MonsterInfo* monsterinfo = nullptr;

	CThreadSync cs;

	StartSearchMonsterinfo();
	while (SearchMonsterinfo(monsterinfo))
	{
		if (monsterinfo->GetMonster()->GetMonster_Code() == _monster_code && monsterinfo->GetMonsterNum() == _monster_num)
		{
			m_monsterinfo_list.remove(monsterinfo);
			delete monsterinfo;
			monsterinfo = nullptr;
			return true;
		}
	}

	return false;
}

// ���� ����Ʈ �ʱ�ȭ(�������� Ŭ���� or ���ο� �����϶� ����Ʈ�� �ʱ�ȭ�ϰ� ���ο� ���� �ֱ��� �۾�)
void MonsterControl::ResetMonsterInfo()
{
	MonsterInfo* target = nullptr;
	MONSTER_ORIGINCODE code;

	for (list<MonsterInfo*>::iterator monsterinfo_save = m_monsterinfo_list.begin(); monsterinfo_save != m_monsterinfo_list.end(); ++monsterinfo_save)
	{
		target = (*monsterinfo_save);
		m_monsterinfo_list.remove(target);
		delete target;
	}

	// �޸𸮿� ���������� �����ؾ���
	m_monstercode_vector.clear();
}

// ����Ʈ ����ִ���(��������� true �ƴϸ� false)
bool MonsterControl::GetMonsterList_Empty()
{
	return m_monsterinfo_list.empty();
}

// �������� ���� ����
void MonsterControl::Stage_SetMonster(int _code[], int _num[])
{
	for (int i = 0; i < STAGE_MONSTER_NUM; i++)
	{
		for (int c = 0; c < _num[i]; c++)
		{
			SetMonsterinfo(_code[i], c);
		}
		AddMonsterCode_vector(_code[i]);
	}
	monster_types_num = m_monstercode_vector.size();
}

// �������� ���� ���� ����
void MonsterControl::Stage_SetBossMonster(int _code, int _num)
{
	// ���� �ʱ�ȭ
	m_boss_on_the_attack = false;
	m_boss_attack_num = 0;
	m_boss_attack_max_time = 0;
	// ���� ����
	SetMonsterinfo(_code, _num);
	AddMonsterCode_vector(_code);
	monster_types_num = m_monstercode_vector.size();
}

// ���� �������
int MonsterControl::GetMonsterNum(int _code)
{
	CThreadSync cs;
	MonsterInfo* target = nullptr;
	int count = 0;
	for (m_monsterinfo_save = m_monsterinfo_list.begin(); m_monsterinfo_save != m_monsterinfo_list.end(); ++m_monsterinfo_save)
	{
		target = (*m_monsterinfo_save);
		if (target->GetMonster()->GetMonster_Code() == _code)
		{
			count++;
		}	
	}
	return count;
}

// �̸��ʹ� ���������ΰ�
bool MonsterControl::Is_BossMonster(int _code)
{
	MonsterInfo* monsterinfo = nullptr;
	if (GetMonsterinfo(_code, 0, monsterinfo) == false)
	{
		return false;
	}

	return monsterinfo->Is_BossMonster();
}

// ���� ���� Ÿ�̸� ����
void MonsterControl::StartBossMonsterTimer()
{
	m_boss_monster_timer.Start_Time();
}

// ���� ���� Ÿ�̸� ����
bool MonsterControl::Is_End_BossMonsterTimer()
{
	double time = m_boss_monster_timer.End_Time();
	if (time < m_boss_attack_max_time)
	{
		return false;
	}
	return true;
}

// ���� ���� ���� ���������� ���� ���� ����(���� ��ȣ,���� �ð�)
void MonsterControl::SetBossMonsterAttackInfo(int _attacknum, int _attacktime)
{
	m_boss_on_the_attack = true;
	m_boss_attack_num = _attacknum;
	m_boss_attack_max_time = _attacktime;
	// Ÿ�̸� ����
	StartBossMonsterTimer();
}

// ���� ü�� ����
bool MonsterControl::Monster_HP_Down(int _monster_code, int _monster_num, int _damage)
{
	MonsterInfo* monsterinfo = nullptr;

	if (GetMonsterinfo(_monster_code, _monster_num, monsterinfo) == false)
	{
		return false;
	}

	CThreadSync cs;

	float hp = monsterinfo->GetMonster()->GetMonster_Current_HP() - _damage;

	// ������ false
	if (hp <= 0)
	{
		monsterinfo->GetMonster()->SetMonster_Current_HP(0);
		monsterinfo->SetMonsterDie();
		return false;
	}
	else // ��������� true
	{
		monsterinfo->GetMonster()->SetMonster_Current_HP(hp);
		return true;
	}
}