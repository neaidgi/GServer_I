#ifndef _RANDOMNUMBER_MANAGER_H_
#define _RANDOMNUMBER_MANAGER_H_

#include "Global.h"

class RandomNumberManager
{
private:
	static RandomNumberManager* Instance;

	RandomNumberManager();
	~RandomNumberManager();

public:
	static void CreateInstance();
	static RandomNumberManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// ���� ���� �ִ��Լ�
	int GetRandomNumber(int _maxnum)
	{
		int randomnum = 0;
		randomnum = rand() % (_maxnum);
		return randomnum;
	}
};

#endif