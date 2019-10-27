#ifndef PARTYROOM_H
#define PARTYROOM_H

#define PARTYMAXUSER 4

#include "MonsterControl.h"
#include "MonsterTime.h"
#include "User.h"
#include <list>

// 파티방
class PartyRoom
{
private:
	// 파티 번호
	int partyroom_num;
	// 파티 유저 리스트
	std::list<User*> m_partyuser;
	std::list<User*>::iterator save;
	// 몬스터 컨트롤러
	MonsterControl* m_monster_control;
	// 스테이지 정보
	StageInfo* m_stage_info;

public:
	PartyRoom();
	PartyRoom(int _partyroomnum);
	~PartyRoom();

	// 초기화
	void InitPartyRoom();
	// 마무리
	void DestroyPartyRoom();

	/* -- 파티 정보 관련 -- */
	// 검색 초기화
	void StartSearchPartyRoom();
	// 파티원(유저) 전달
	bool SearchPartyRoom(User*& _user);
	// 파티방 번호
	int GetPartyRoomNum();
	// 현재 파티원 인원수
	int GetPartRoomUserNum();
	// 유저 추가
	void PartyRoomAddUser(User* _user);
	// 유저 추가(코드)
	// -- 안만듬
	// 유저 삭제
	bool PartyRoomRemoveUser(User* _user);
	// 유저 삭제(코드)
	bool PartyRoomRemoveUser(char* _code);
	// 파티장 임명
	bool AppointPartyLeader(User* _user);
	// 파티장 임명
	bool AppointPartyLeader(char* _code);
	// 파티장 해임
	bool DismissPartyLeader(User* _user);
	// 파티장 해임
	bool DismissPartyLeader(char* _code);
	// 파티장이 누구인가
	User* WhopartyLeader();

	/* -- 파티 정보 관련 -- */
	// 던전 입장
	void DungeonEnter_PartyRoom();
	// 던전 퇴장
	void DungeonLeave_PartyRoom();

	/* -- 스테이지 관련 -- */
	// 스테이지 변경
	void SetDungeonStage(int _stage_num) { m_stage_info->SetStage_Num(_stage_num); }
	// 지금 몇번째 스테이지인지
	int GetDungeonStage() { return m_stage_info->GetStage_Num(); }
	// 스테이지 상승
	void RiseStage();
	// 스테이지에 맞는 몬스터 정보 셋팅
	void SetDungeonMonsterinfo();
	// 지금 보스 스테이지인지
	bool GetIsBossStage() { return m_stage_info->GetStage_Is_Boss_Stage(); }

	/* -- 몬스터 타이머 관련 -- */
	// 몬스터 시간 초기화
	void Start_MonsterTimer(int _code, int _num);
	// 몬스터 시간 지났는지 확인
	bool End_MonsterTimer(int _code, int _num);

	/* -- 몬스터 정보 관련 -- */
	// 몬스터 종류는 몇마리인지
	int GetMonsterTypes() { return m_monster_control->GetMonsterTypes(); }
	// 몬스터 검색 초기화
	void StartSearchMonsterinfo();
	// 몬스터 검색
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	// 몬스터 정보 요청
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// 몬스터 저장(몬스터코드,몬스터번호) - 새롭게 저장할때
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표) - 기본정보에서 수정할때
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos);
	// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표,몬스터방향) - 기본정보에서 수정할때
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _dir);
	// 몬스터 정보 삭제
	bool RemoveMonsterInfo(int _monster_code, int _monster_num);
	// 몬스터 체력 감소. 죽으면 false. 살아있으면 true
	bool Monster_HP_Down(int _monster_code, int _monster_num, int _damage);
	// 몬스터 코드 반환
	int GetMonsterCode(int _code);
	// 특정 몬스커 몇마리 인지
	int GetMonsterNum(int _code);

	/* -- 보스 몬스터 관련 -- */
	// 이몬스터가 보스몬스터인지
	bool Is_BossMonster(int _code);
	// 보스 몬스터 현재 공격했을때 공격 정보 저장(공격 번호,공격 시간)
	void SetBossMonsterAttackInfo(int _attacknum, int _attacktime);
	// 보스 몬스터 공격 번호 가져오기
	int GetBossMonsterAttackNum() { return m_monster_control->GetBossMonsterAttackNum(); }
	// 보스 몬스터가 공격 가능한가
	bool Is_BossMonster_Attackable() { return m_monster_control->Is_BossMonster_Attackable(); }
	// 보스 몬스터는 무슨 공격을 할것인가(몬스터코드,길이,[아웃풋]공격번호)
	bool Is_BossMonster_What_Attack(MonsterInfo* _monster, float _length, int& _attackcode);
};

#endif
