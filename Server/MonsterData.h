#ifndef _MONSTERDATA_H_
#define _MONSTERDATA_H_

#include "Monster.h"

class MonsterData {
private:
	// 몬스터 기본 정보
	Monster * m_Monster_origin[MAXMONSTERORIGIN];
	bool isLoad;
public:
	MonsterData();
	~MonsterData();

	void SetMonsterOrigin(Monster* _origin[]);

	const Monster* GetSpider() { return m_Monster_origin[0]; }
	const Monster* GetWorm() { return m_Monster_origin[1]; }
	const Monster* GetBossSpider() { return m_Monster_origin[2]; }
	const Monster* GetDog() { return m_Monster_origin[3]; }
	const Monster* GetOrcCannonSoldier() { return m_Monster_origin[4]; }
	const Monster* GetKingOfTheAxe() { return m_Monster_origin[5]; }
	const Monster* GetBear() { return m_Monster_origin[6]; }
	const Monster* GetDinosaur() { return m_Monster_origin[7]; }
	const Monster* GetDragon() { return m_Monster_origin[8]; }
};

#endif