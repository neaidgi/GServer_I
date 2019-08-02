#include "CharacterVerification.h"

// 초기화
void CharacterVerification::Initialize(float _CommuTime, float _Move_MarginofError)
{
	CommuTime = _CommuTime;
	Move_MarginofError = _Move_MarginofError;
	m_collision = new Collision();
}

// 이동 검증 함수
bool CharacterVerification::CharacterMoveVerificate(Vector3 _curPos, Vector3 _prePos, int _distance)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	int range = m_collision->Distance(_curPos, _prePos);

	if (m_collision->Distance(_curPos, _prePos) > _distance)
	{
		sprintf(msg, "[이동 판정 : 이동 실패]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return false;
	}
	sprintf(msg, "[이동 판정 : 이동 성공]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	return true;
}

// 공격 판정(공격한자 위치, 공격범위, 타겟 위치, 타겟 크기)
bool CharacterVerification::AttackVerificate(Vector3 _AttackerPos, float _attackerRange, Vector3 _targetPos, float _targetRange)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	// 원의 중심간의 거리 충돌
	if (m_collision->Circle_IsCollision(_AttackerPos, _attackerRange, _targetPos, _targetRange) == false)
	{
		/*sprintf(msg, "[공격 판정 : 원과의 거리에서 충돌하지않음]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/

		return false;
	}
	return true;
}

// 공격 판정(공격하는자 위치, 공격 방향, 공격범위, 공격 범위 각도, 타겟 위치,타겟 크기)
bool CharacterVerification::AttackVerificate(Vector3 _AttackerPos, Vector3 _attackerDir, float _attackerRange, int _attackerAngle, Vector3 _targetPos, float _targetRange)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	// 원의 중심간의 거리 충돌
	if (m_collision->Circle_IsCollision(_AttackerPos, _attackerRange, _targetPos, _targetRange) == false)
	{
		/*sprintf(msg, "[공격 판정 : 원과의 거리에서 충돌하지않음]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/
		return false;
	}

	//sprintf(msg, "[공격 판정 : 캐릭터위치 [x : %f] [y : %f] [z : %f], 공격판정위치 [x : %f] [y : %f] [z : %f], 몬스터위치 [x : %f] [y : %f] [z : %f]]"
	//, _AttackerPos.x, _AttackerPos.y, _AttackerPos.z, _attackerDir.x, _attackerDir.y, _attackerDir.z, _targetPos.x, _targetPos.y, _targetPos.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 공격자에서 타겟의 방향 벡터 구하기
	Vector3 attackerPos = Vector3(_AttackerPos.x, _AttackerPos.y, 0);
	Vector3 targetPos = Vector3(_targetPos.x, _targetPos.y,0);
	Vector3 attackerDir = Vector3(_attackerDir.x, _attackerDir.y, 0);
	// 좌표를 캐릭터 중심으로 옮기기
	targetPos = Vector3(targetPos.x - attackerPos.x, targetPos.y - attackerPos.y, 0);
	attackerDir = Vector3(attackerDir.x - attackerPos.x, attackerDir.y - attackerPos.y, 0);
	attackerPos = Vector3(attackerPos.x - attackerPos.x, attackerPos.y - attackerPos.y, 0);

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "[공격 판정 : 캐릭터위치 [x : %f] [y : %f] [z : %f], 공격판정위치 [x : %f] [y : %f] [z : %f], 몬스터위치 [x : %f] [y : %f] [z : %f]]"
	//	, attackerPos.x, attackerPos.y, attackerPos.z, attackerDir.x, attackerDir.y, attackerDir.z, targetPos.x, targetPos.y, targetPos.z);

	if (m_collision->FanShape_IsCollision(attackerDir, targetPos, _attackerAngle) == false)
	{
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "[공격 판정 : 공격사거리에 충돌하지않음]");
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return false;
	}

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "[공격 판정 : 공격 성공]");
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	return true;
}
