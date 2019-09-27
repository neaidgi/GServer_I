#ifndef _RANDOMNUMBER_MANAGER_H_
#define _RANDOMNUMBER_MANAGER_H_

#include "Global.h"
#include "GameDataManager.h"

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

	// 랜덤 숫자 주는함수
	int GetRandomNumber(int _maxnum)
	{
		int randomnum = 0;
		randomnum = rand() % (_maxnum);
		return randomnum;
	}

	// 가중치 랜덤
	MonsterNowAttackInfo GetRandom_MonsterAttackInfo(std::vector<MonsterNowAttackInfo> attackinfo)
	{
		int num = 0;
		int maxnum = 0;
		int weight = 0;
		int index = 0;

		for (int i = 0; i < attackinfo.size(); i++)
		{
			maxnum += attackinfo.at(i).weight;
		}
		num = GetRandomNumber(maxnum);

		for (int i = 0; i < attackinfo.size(); i++)
		{
			weight += attackinfo.at(i).weight;
			if (num < weight)
			{
				index = i;
				break;
			}
		}
		return attackinfo.at(index);
	}
};

#endif