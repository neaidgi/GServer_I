#include "MonsterTime.h"

void MonsterTime::Start_Time()
{
	start = clock();
}

int MonsterTime::End_Time()
{
	end = clock();

	result = (end - start) / CLOCKS_PER_SEC;;

	return 	result; ;
}
