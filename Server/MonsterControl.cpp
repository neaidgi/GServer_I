#include "MonsterControl.h"

MonsterControl::MonsterControl()
{
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

void MonsterControl::StartSearchMonsterinfo()
{
	m_monsterinfo_save = m_monsterinfo_list.begin();
}

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

// 몬스터 정보 검색
bool MonsterControl::GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo *& _monsterinfo)
{
	MonsterInfo* monsterinfo = nullptr;

	StartSearchMonsterinfo();
	while (SearchMonsterinfo(monsterinfo))
	{
		if (monsterinfo->m_monster->GetMonster_Code() == _monster_code && monsterinfo->m_monster_info_num == _monster_num)
		{
			_monsterinfo = monsterinfo;
			return true;
		}
	}

	return false;
}
