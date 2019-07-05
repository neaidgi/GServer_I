#pragma once

#include "User.h"
#include "Collision.h"
#include "MsgManager.h"

// ĳ���� �����۾��� ����ϴ� ��ü

class CharacterVerification 
{
private:
	float Move_MarginofError;		// �̵� ���� ����
	float CommuTime;				// ��� �ð�
	Collision* m_collision;			// �浹 ó�����
public:
	CharacterVerification(){}
	~CharacterVerification(){}

	void Initialize(float _CommuTime, float _Move_MarginofError);								// �ʱ�ȭ
	bool CharacterMoveVerificate(Vector3 _curPos, Vector3 _prePos, float _speed);				// ĳ���� �̵� ����
	// ���� ����(�����ϴ��� ��ġ, ���� ����, ���ݹ���, ���� ���� ����, Ÿ�� ��ġ,Ÿ�� ũ��)
	bool AttackVerificate(Vector3 _AttackerPos, Vector3 _attackerDir, float _attackerRange, int _attackerAngle, Vector3 _targetPos, float _targetRange);
}; 