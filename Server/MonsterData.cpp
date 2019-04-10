#include "MonsterData.h"

MonsterData::MonsterData()
{
	memset(m_Monster_origin, 0, sizeof(m_Monster_origin));
	isLoad = false;
}

MonsterData::~MonsterData()
{
	if (isLoad)
	{
		for (int i = 0; i < MAXMONSTERORIGIN; i++)
		{
			delete m_Monster_origin[i];
		}
	}
}

void MonsterData::SetMonsterOrigin(Monster * _origin[])
{
	for (int i = 0; i < MAXMONSTERORIGIN; i++)
	{
		m_Monster_origin[i] = _origin[i];
	}
	isLoad = true;
}
