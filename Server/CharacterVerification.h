#pragma once

#include "User.h"

// ĳ���� �����۾��� ����ϴ� ��ü

class CharacterVerification {
private:
	float Move_MarginofError;		// �̵� ���� ����
	float CommuTime;				// ��� �ð�
public:
	CharacterVerification(){}
	~CharacterVerification(){}

	void Initialize(float CommuTime, float _Move_MarginofError);								// �ʱ�ȭ
	bool CharacterMoveVerificate(Vector3 _curPos, Vector3 _prePos, float _speed);				// ĳ���� �̵� ����
};