#ifndef MONSTER_TIME_H
#define MONSTER_TIME_H

#include "Global.h"

// ���� ������ ������ üũ�� �ð�
class MonsterTime
{
private:
	clock_t start, end;
	double result;

public:
	MonsterTime() { result = 0; }
	~MonsterTime() {}

	void Start_Time();
	double End_Time();
};

#endif