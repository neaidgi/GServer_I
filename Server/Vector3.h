#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "Global.h"

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3() 
	{ 
		x = 0; y = 0; z = 0; 
	}
	Vector3(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}

	// = 대입
	Vector3 operator=(const Vector3& _v)
	{
		return Vector3(x = _v.x, y = _v.y, z = _v.z);
	}

	// +
	Vector3 operator+(const Vector3& _v)
	{
		return Vector3(x + _v.x, y + _v.y, z + _v.z);
	}

	// -
	Vector3 operator-(const Vector3& _v)
	{
		return Vector3(x + (_v.x * -1), y + (_v.y * -1), z + (_v.z * -1));
	}

	// 스칼라 곱
	Vector3 operator*(const int _k)
	{
		return Vector3(x * _k, y * _k, z * _k);
	}

	// 내적
	float DotProduct(const Vector3& _v)
	{
		return (x*_v.x + y * _v.y + z * _v.z);
	}

	Vector3 Projection(Vector3& _v)
	{
		Vector3 n = _v.nomalize();
		return Vector3(n * DotProduct(n));
	}

	// 외적
	Vector3 CrossProduct(const Vector3& _v)
	{
		return Vector3((y*_v.z - z * _v.y), (z*_v.x - x * _v.z), (x*_v.y - y * _v.x));
	}

	// == 비교
	bool operator==(const Vector3& _v)
	{
		if (x == _v.x && y == _v.y && z == _v.z)
			return true;
		else
			return false;
	}
	
	// 정규화
	Vector3 nomalize()
	{
		return Vector3(x / length(), y / length(), z / length());
	}

	// ||V|| 길이
	float length()
	{
		return sqrt((x * x + y * y + z * z));
	}

	// 두 벡터 사이의 거리 구하기
	float Distance(const Vector3& _a, const Vector3& _b)
	{
		return sqrt(pow(_a.x - _b.x, 2) + pow(_a.y - _b.y, 2) + pow(_a.z - _b.z, 2));
	}
};

#endif