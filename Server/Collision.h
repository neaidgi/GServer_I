#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "Global.h"

class Collision
{
public:

	// 두 벡터 사이의 거리 구하기
	float Distance(const Vector3 _a, const Vector3 _b)
	{
		return sqrt(pow(_a.x - _b.x, 2) + pow(_a.y - _b.y, 2) + pow(_a.z - _b.z, 2));
	}

	// 원과 원의 충돌
	bool Circle_IsCollision(Vector3 _a, float _a_length, Vector3 _b, float _b_length)
	{
		float tempX = _a.x - _b.x;
		float tempY = _a.y - _b.y;

		float length = sqrt(tempX * tempX + tempY * tempY);

		// 원의 중심간의 거리가 두원의 반지름의 합보다 크면 충돌하지않음
		if (length > (_a_length + _b_length))
		{
			return false;
		}
		return true;
	}

	// 부채꼴 충돌
	bool FanShape_IsCollision(Vector3 _a, Vector3 _b, float _angle)
	{
		// 내적
		float a = (_a.x * _b.x + _a.y * _b.y + _a.z * _b.z);

		float angle = acos(a / (_a.length() * _b.length()));
		angle = angle * RAD2DEG;

		// 시야 각 안에 들어오는가
		if (angle <= (_angle/2))
		{
			return true;
		}
		return false;
	}
};

#endif