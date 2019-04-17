#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "GameDataManager.h"
#include "User.h"
#include "CriticalSectionManager.h"

// 클라로 부터 받는 몬스터 정보(몇번째정보인지,몬스터객체,활성화되어있는지아닌지)
struct MonsterInfo
{
private:
	Monster* m_monster;
	int m_monster_info_num;
	bool m_monster_activate;
public:
	MonsterInfo()
	{
		m_monster_info_num = -1;
		m_monster = nullptr;
		m_monster_activate = false;
	}
	~MonsterInfo()
	{
		delete m_monster;
		m_monster = nullptr;
	}
	// 몬스터 접근지정자
	void SetMonster(Monster* _monster) { m_monster = _monster; }
	Monster* GetMonster() { return m_monster; }

	// 몬스터 번호 접근지정자
	void SetMonsterNum(int _num) { m_monster_info_num = _num; }
	int GetMonsterNum() { return m_monster_info_num; }

};

// 몬스터 객체를 컨트롤할 클래스
class MonsterControl
{
private:
	// 몬스터 정보
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;

	// 몬스터 종류 갯수
	int number_monster_types;
public:

	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	
	// 몬스터 실제 조립
	Monster* MonsterSelect(int _monster_code);
	// 몬스터 정보 검색
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// 몬스터 저장(몬스터코드,몬스터번호) - 새롭게 저장할때
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표,몬스터회전값) - 기본정보에서 수정할때
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _rot);
	// 몬스터 리스트 초기화(스테이지 클리어 or 새로운 상태일때 리스트를 초기화하고 새로운 몹을 넣기전 작업)
	void ResetMonsterInfo();
	// 리스트 비어있는지(비어있으면 true 아니면 false)
	bool GetMonsterList_Empty();
	// 첫번째 스테이지 일반몹 저장
	void SetFirstStage_NormalMonster();
};

#endif