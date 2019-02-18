#pragma once

#include "User.h"

// 캐릭터 검증작업을 담당하는 객체

class CharacterVerification {
private:
	float Move_MarginofError;		// 이동 오차 범위
	float CommuTime;				// 통신 시간
public:
	CharacterVerification(){}
	~CharacterVerification(){}

	void Initialize(float CommuTime, float _Move_MarginofError);								// 초기화
	bool CharacterMoveVerificate(Vector3 _curPos, Vector3 _prePos, float _speed);				// 캐릭터 이동 검증
};