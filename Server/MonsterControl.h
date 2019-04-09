#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "Monster.h"
#include "User.h"
#include "CriticalSectionManager.h"

// Ŭ��� ���� �޴� ���� ����(���°��������,���Ͱ�ü,Ȱ��ȭ�Ǿ��ִ����ƴ���)
struct MonsterInfo
{
	Monster* m_monster;
	int m_monster_info_num;
	bool m_monster_activate;

	MonsterInfo()
	{
		m_monster_info_num = -1;
		m_monster = nullptr;
		m_monster_activate = true;
	}
	~MonsterInfo()
	{

	}
};

// ���� ��ü�� ��Ʈ���� Ŭ����
class MonsterControl
{
private:
	// ���� ����
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;
public:

	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
		
	// ���� ���� �˻�
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// ���� ����
	//void SetMonsterinfo()
};

#endif