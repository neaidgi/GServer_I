#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "GameDataManager.h"
#include "User.h"
#include "CriticalSection.h"
#include "RandomNumberManager.h"
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
	MonsterTime* GetMonsterTime() { return m_monster_time; }

	// 몬스터 타이머 생성
	void CreateMonsterTimer();
	// 몬스터 타이머 시간 초기화
	void ResetMonsterTime();
	// 몬스터 시간이 지났는지
	bool Is_End_MonsterTime();
	// 이몬스터는 보스몬스터인가
	bool Is_BossMonster();
};

// 몬스터 객체를 컨트롤할 클래스
class MonsterControl : public CMultiThreadSync<MonsterControl>
{
private:
	// -- 보스몬스터 관련 -- //
	// 보스 몬스터 공격 타이머
	MonsterTime m_boss_monster_timer;
	// 보스 몬스터가 공격 중인가
	bool m_boss_on_the_attack;
	// 보스 몬스터 공격 번호
	int m_boss_attack_num;
	// 보스 몬스터 공격 시간
	float m_boss_attack_max_time;

	// -- 몬스터 정보 자료구조 -- //
	// 몬스터 정보
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;

	// 몬스터 종류 갯수
	int monster_types_num;
	std::vector<int> m_monstercode_vector;
	std::vector<int>::iterator m_monstercode_save;

	/* -- 보스 몬스터 관련 함수 -- */
	// 보스 몬스터 타이머 시작
	void StartBossMonsterTimer();
	// 보스 몬스터 타이머 종료
	bool Is_End_BossMonsterTimer();
public:
	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	
	// 몬스터 종류는 몇마리인가
	int GetMonsterTypes() {	return m_monstercode_vector.size(); }
	
	// 몬스터 코드 추가
	void AddMonsterCode_vector(int _code);
	// 몬스터 코드 반환
	int GetMonsterCode_vector(int _count);

	// 몬스터 실제 조립
	Monster* MonsterSelect(int _monster_code);

	// -- 몬스터 타이머 -- //
	// 몬스터 타이머 시간초기화
	void InitMonsterTime(int _code, int _num);
	// 몬스터 시간이 지났는지
	bool Is_End_MonsterTime(int _code, int _num);

	/* -- 몬스터 정보 저장,수정,검색,삭제 -- */
	// 몬스터 정보 검색
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// 몬스터 저장(몬스터코드,몬스터번호) - 새롭게 저장할때
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표) - 기본정보에서 수정할때
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos);
	// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표,몬스터방향) - 기본정보에서 수정할때
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _dir);
	// 몬스터 정보 삭제
	bool RemoveMonsterInfo(int _monster_code, int _monster_num);
	// 몬스터 리스트 초기화(스테이지 클리어 or 새로운 상태일때 리스트를 초기화하고 새로운 몹을 넣기전 작업)
	void ResetMonsterInfo();
	// 리스트 비어있는지(비어있으면 true 아니면 false)
	bool GetMonsterList_Empty();
	// 몬스터 몇마리인지
	int GetMonsterNum(int _code);
	// 몬스터 체력 감소
	bool Monster_HP_Down(int _monster_code, int _monster_num, int _damage);

	/* -- 스테이지 관련 -- */
	// 스테이지 몬스터 저장
	void Stage_SetMonster(int _code[],int _num[]);

	/* -- 보스몬스터 관련 -- */
	// 스테이지 보스 몬스터 저장
	void Stage_SetBossMonster(int _code, int _num);
	// 이몬스터는 보스몬스터인가
	bool Is_BossMonster(int _code);
	// 보스 몬스터 현재 공격했을때 공격 정보 저장(공격 번호,공격 시간)
	void SetBossMonsterAttackInfo(int _attacknum, int _attacktime);
	// 보스 몬스터 공격 번호 가져오기
	int GetBossMonsterAttackNum() {	return m_boss_attack_num; }
	// 보스 몬스터가 공격 가능한가
	bool Is_BossMonster_Attackable() { return Is_End_BossMonsterTimer(); }
	// 보스 몬스터는 무슨 공격을 할것인가(몬스터코드,길이,[아웃풋]공격번호)
	bool Is_BossMonster_What_Attack(MonsterInfo* _monster, float _length, int& _attackcode);


	// 보스몬스터 변수 초기화, 공격코드 getset,공격중인지, 공격 시간 getset
};

#endif