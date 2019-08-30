#pragma once

#include "User.h"
#include "Collision.h"
#include "MsgManager.h"
#include "PartySystem.h"

// 캐릭터 검증작업을 담당하는 객체

class CharacterVerification 
{
private:
	float Move_MarginofError;		// 이동 오차 범위
	float CommuTime;				// 통신 시간
	Collision* m_collision;			// 충돌 처리담당
public:
	CharacterVerification(){}
	~CharacterVerification(){}

	void Initialize(float _CommuTime, float _Move_MarginofError);								// 초기화
	// 캐릭터 이동 검증
	bool CharacterMoveVerificate(Vector3 _curPos, Vector3 _prePos, int _distance);				
	// 공격 판정(공격한자 위치, 공격범위, 타겟 위치, 타겟 크기) - 타입3(폭발형)
	bool AttackVerificate(Vector3 _AttackerPos, float _attackerRange, Vector3 _targetPos, float _targetRange);
	// 공격 판정(공격하는자 위치, 공격 방향, 공격범위, 공격 범위 각도, 타겟 위치,타겟 크기) - 타입 1(근접공격형)
	bool AttackVerificate(Vector3 _AttackerPos, Vector3 _attackerDir, float _attackerRange, int _attackerAngle, Vector3 _targetPos, float _targetRange);
	// 공격 가능한 거리인가
	bool Is_Attackable_Distance(Vector3 _a, Vector3 _b, float _length);
	// 가장 가까운 캐릭터는 누구인가 - 주의 - 사용하는 쪽에서 동기화를 꼭 사용하자
	float Is_Nearest_Chracter(PartyRoom* _partyroom, MonsterInfo* _monster, Character*& _character);
	// 방향 벡터로 전환
	//Vector3 Change_Direction_Vector(Vector3 _a, Vector3 _b);
}; 