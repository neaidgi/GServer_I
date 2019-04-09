#ifndef _MONSTERDATA_H_
#define _MONSTERDATA_H_

#include "Monster.h"

class MonsterData {
private:
	// 몬스터 기본 정보
	Monster * m_Monster_origin[20];
	bool isLoad;
public:
	MonsterData();
	~MonsterData();

	void SetMonsterOrigin(Monster* _origin[]);

	const Monster* GetSpider() { return m_Monster_origin[0]; }
	const Monster* GetWorm() { return m_Monster_origin[1]; }
	const Monster* GetBossSpider() { return m_Monster_origin[2]; }
};

#endif