#include "MonsterTime.h"

void MonsterTime::Start_Time()
{
	start = clock();
}

double MonsterTime::End_Time()
{
	end = clock();

	result = (double)(end - start) / CLOCKS_PER_SEC;;

	return 	result; ;
}
