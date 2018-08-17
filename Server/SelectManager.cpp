#include "SelectManager.h"
#include "ThreadManager.h"
#include "MainManager.h"


void SelectManager::SelectProcess(void* arg)
{
	DWORD id = GetCurrentThreadId();
	SelectGroup* group = (SelectGroup*)ThreadManager::GetInstance()->getData(id);

	int retval;

	FD_SET read;
	FD_SET write;

	timeval t;
	t.tv_sec = 1;
	t.tv_usec = 1000;

	while (group->sock.size() != 0)
	{
		FD_ZERO(&read); //초기화
		FD_ZERO(&write);

		set(&read, &write);

		retval = select(0, &read, &write, NULL, &t);//신호 대기

		if (!recvProcess(read))
		{
			continue;
		}

		if (!sendProcess(write))
		{
			continue;
		}
	}

	ThreadManager::GetInstance()->removeThread(group->thread);
	delete group;
}