#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "Monster.h"
#include "User.h"
#include "CriticalSectionManager.h"

// 클라로 부터 받는 몬스터 정보(몇번째정보인지,몬스터객체,활성화되어있는지아닌지)
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

// 몬스터 객체를 컨트롤할 클래스
class MonsterControl
{
private:
	// 몬스터 정보
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;
public:

	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
		
	// 몬스터 정보 검색
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// 몬스터 저장
	//void SetMonsterinfo()
};

#endif