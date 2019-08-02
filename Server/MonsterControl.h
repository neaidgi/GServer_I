#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "GameDataManager.h"
#include "User.h"
#include "CriticalSectionManager.h"
#include "MonsterTime.h"

// 클라로 부터 받는 몬스터 정보(몇번째정보인지,몬스터객체,활성화되어있는지아닌지)
struct MonsterInfo
{
private:	
	// 몬스터 타이머
	MonsterTime* m_monster_time;
	// 몬스터
	Monster* m_monster;
	// 몬스터 번호
	int m_monster_info_num;
	// 몬스터가 죽었는지 살았는지. 
	bool m_monster_activate;
public:
	MonsterInfo();
	~MonsterInfo();
	// 몬스터 접근지정자
	void SetMonster(Monster* _monster) { m_monster = _monster; }
	Monster* GetMonster() { return m_monster; }

	// 몬스터 번호 접근지정자
	void SetMonsterNum(int _num) { m_monster_info_num = _num; }
	int GetMonsterNum() { return m_monster_info_num; }

	// 몬스터 상태(죽었는지 살았는지) 접근지정자
	void SetMonsterDie() { m_monster_activate = false; }
	void SetMonsterlive() { m_monster_activate = true; }
	bool GetMonsterActivate() { return m_monster_activate; }

	// 몬스터 타이머 접근지정자
	void SetMonsterTime(MonsterTime* _monstertime) { m_monster_time = _monstertime; }
	MonsterTime* GetMonsterTime() { return m_monster_time; }

	// 몬스터 타이머 시간초기화
	void InitMonsterTime();
	// 몬스터 시간이 지났는지
	bool Is_End_MonsterTime();

};

// 몬스터 객체를 컨트롤할 클래스
class MonsterControl : public CMultiThreadSync<MonsterControl>
{
private:
	// 몬스터 정보
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;

	// 몬스터 종류 갯수
	int monster_types_num;
	std::vector<int> m_monstercode_vector;
	std::vector<int>::iterator m_monstercode_save;
public:
	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	
	int GetMonsterTypes() {	return m_monstercode_vector.size(); }
	
	// 몬스터 코드 추가
	void AddMonsterCode_vector(int _code);
	// 몬스터 코드 반환
	int GetMonsterCode_vector(int _count);

	// 몬스터 실제 조립
	Monster* MonsterSelect(int _monster_code);
	// 몬스터 타이머 추가
	MonsterTime* AddMonsterTimer();

	// 몬스터 타이머 시간초기화
	void InitMonsterTime(int _code, int _num);
	// 몬스터 시간이 지났는지
	bool Is_End_MonsterTime(int _code, int _num);

	// 몬스터 정보 검색
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// 몬스터 저장(몬스터코드,몬스터번호) - 새롭게 저장할때
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표,몬스터회전값) - 기본정보에서 수정할때
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos);
	// 몬스터 정보 삭제
	bool RemoveMonsterInfo(int _monster_code, int _monster_num);
	// 몬스터 리스트 초기화(스테이지 클리어 or 새로운 상태일때 리스트를 초기화하고 새로운 몹을 넣기전 작업)
	void ResetMonsterInfo();
	// 리스트 비어있는지(비어있으면 true 아니면 false)
	bool GetMonsterList_Empty();
	// 스테이지 몬스터 저장
	void Stage_SetMonster(int _code[],int _num[]);
	// 스테이지 보스 몬스터 저장
	void Stage_SetBossMonster(int _code, int _num);
	// 몬스터 몇마리인지
	int GetMonsterNum(int _code);
	// 몬스터 체력 감소
	bool Monster_HP_Down(int _monster_code, int _monster_num, int _damage);
};

#endif