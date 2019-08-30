#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "Global.h"

class Collision
{
public:

	// �� ���� ������ �Ÿ� ���ϱ�
	float Distance(const Vector3 _a, const Vector3 _b)
	{
		return sqrt(pow(_a.x - _b.x, 2) + pow(_a.y - _b.y, 2) + pow(_a.z - _b.z, 2));
	}

	// ���� ���� �浹
	bool Circle_IsCollision(Vector3 _a, float _a_length, Vector3 _b, float _b_length)
	{
		float tempX = _a.x - _b.x;
		float tempY = _a.y - _b.y;

		float length = sqrt(tempX * tempX + tempY * tempY);

		// ���� �߽ɰ��� �Ÿ��� �ο��� �������� �պ��� ũ�� �浹��������
		if (length > (_a_length + _b_length))
		{
			return false;
		}
		return true;
	}

	// ��ä�� �浹
	bool FanShape_IsCollision(Vector3 _a, Vector3 _b, float _angle)
	{
		// ����
		float a = (_a.x * _b.x + _a.y * _b.y + _a.z * _b.z);

		float angle = acos(a / (_a.length() * _b.length()));
		angle = angle * RAD2DEG;

		// �þ� �� �ȿ� �����°�
		if (angle <= (_angle/2))
		{
			return true;
		}
		return false;
	}
};

#endif