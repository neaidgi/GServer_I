#pragma once

#include "User.h"
#include "Collision.h"
#include "MsgManager.h"

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
}; 