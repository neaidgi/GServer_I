#pragma once

#include "User.h"
#include "Collision.h"
#include "MsgManager.h"
#include "PartySystem.h"

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
	// ĳ���� �̵� ����
	bool CharacterMoveVerificate(Vector3 _curPos, Vector3 _prePos, int _distance);				
	// ���� ����(�������� ��ġ, ���ݹ���, Ÿ�� ��ġ, Ÿ�� ũ��) - Ÿ��3(������)
	bool AttackVerificate(Vector3 _AttackerPos, float _attackerRange, Vector3 _targetPos, float _targetRange);
	// ���� ����(�����ϴ��� ��ġ, ���� ����, ���ݹ���, ���� ���� ����, Ÿ�� ��ġ,Ÿ�� ũ��) - Ÿ�� 1(����������)
	bool AttackVerificate(Vector3 _AttackerPos, Vector3 _attackerDir, float _attackerRange, int _attackerAngle, Vector3 _targetPos, float _targetRange);
	// ���� ������ �Ÿ��ΰ�
	bool Is_Attackable_Distance(Vector3 _a, Vector3 _b, float _length);
	// ���� ����� ĳ���ʹ� �����ΰ� - ���� - ����ϴ� �ʿ��� ����ȭ�� �� �������
	float Is_Nearest_Chracter(PartyRoom* _partyroom, MonsterInfo* _monster, Character*& _character);
	// ���� ���ͷ� ��ȯ
	//Vector3 Change_Direction_Vector(Vector3 _a, Vector3 _b);
}; 