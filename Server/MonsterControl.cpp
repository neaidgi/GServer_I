#include "MonsterControl.h"

MonsterControl::MonsterControl()
{
	number_monster_types = 0;
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
void MonsterControl::AddMonsterCode_vector(MONSTER_ORIGINCODE _code)
{
	m_monstercode_vector.push_back(_code);
}

// ���� �ڵ� ��ȯ
MONSTER_ORIGINCODE MonsterControl::GetMonsterCode_vector(int _count)
{
	return m_monstercode_vector.at(_count);
}

// ���� ���� ����
Monster * MonsterControl::MonsterSelect(int _monster_code)
{
	Monster* monster = new Monster();
	const Monster* origin_monster = nullptr;
	GameDataManager::GetInstance()->Monster_Origin_Data(_monster_code, origin_monster);

	monster->SetMonster_AttackPoint(origin_monster->GetMonster_AttackPoint());
	monster->SetMonster_Code(origin_monster->GetMonster_Code());
	monster->SetMonster_DefensePoint(origin_monster->GetMonster_DefensePoint());
	monster->SetMonster_Health(origin_monster->GetMonster_Health());
	monster->SetMonster_Mana(origin_monster->GetMonster_Mana());
	monster->SetMonster_Name(origin_monster->GetMonster_Name());
	monster->SetMonster_Num(origin_monster->GetMonster_Num());
	monster->SetMonster_Speed(origin_monster->GetMonster_Speed());
	monster->SetPosition(origin_monster->GetPosition());
	monster->SetRotation(origin_monster->GetRotation());
	monster->SetScale(origin_monster->GetScale());

	return monster;
}

// ���� ���� �˻�
bool MonsterControl::GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo *& _monsterinfo)
{
	MonsterInfo* monsterinfo = nullptr;

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

	m_monsterinfo_list.push_back(monsterinfo);
}

// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,����ȸ����) - �⺻�������� �����Ҷ�
void MonsterControl::SetMonsterinfo(int _monster_code, int _monster_num,const Vector3 _pos)
{
	MonsterInfo* monsterinfo = nullptr;
	GetMonsterinfo(_monster_code, _monster_num, monsterinfo);
	monsterinfo->GetMonster()->SetPosition(_pos);
	//monsterinfo->GetMonster()->SetRotation(_rot);
}

// ���� ����Ʈ �ʱ�ȭ(�������� Ŭ���� or ���ο� �����϶� ����Ʈ�� �ʱ�ȭ�ϰ� ���ο� ���� �ֱ��� �۾�)
void MonsterControl::ResetMonsterInfo()
{
	MonsterInfo* target = nullptr;
	MONSTER_ORIGINCODE code;

	for (m_monsterinfo_save = m_monsterinfo_list.begin(); m_monsterinfo_save != m_monsterinfo_list.end(); ++m_monsterinfo_save)
	{
		target = (*m_monsterinfo_save);
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

// ù��° �������� �Ϲݸ� ����
void MonsterControl::SetFirstStage_NormalMonster()
{
	// �Ź� ����
	for (int i = 0; i < FIRSTSTAGE_NORMALMONSTER_1; i++)
	{
		SetMonsterinfo(SPIDER, i);
	}
	// �ֹ��� ����
	//for (int i = 0; i < FIRSTSTAGE_NORMALMONSTER_1; i++)
	//{
	//	SetMonsterinfo(WORM, i);
	//}

	AddMonsterCode_vector(SPIDER);
	//AddMonsterCode_vector(WORM);

	/*number_monster_types = m_monstercode_vector.size();*/
}
