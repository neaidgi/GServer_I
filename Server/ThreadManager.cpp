#include "ThreadManager.h"
#include "MainManager.h"
#include "GameManager.h"

ThreadManager* ThreadManager::Instance = nullptr;

ThreadManager::ThreadManager()
{
}
ThreadManager::~ThreadManager()
{

}

void ThreadManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new ThreadManager();
	}
}
ThreadManager* ThreadManager::GetInstance()
{
	return Instance;
}
void ThreadManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool ThreadManager::InitializeManager()
{
	return true;
}

void ThreadManager::EndManager()
{

}

HANDLE ThreadManager::addThread(LPTHREAD_START_ROUTINE routine,LPVOID data, LPVOID _pthis)
{
	HANDLE handle = CreateThread(NULL, 0, routine, _pthis, 0, NULL);
	if (handle == NULL) {} 
	ThreadData* t = new ThreadData();
	t->data = data;
	t->thread = handle;
	threadInfo.push_back(t);
	return handle;
}

// ������ ����
void ThreadManager::removeThread(HANDLE thread)
{
	int i;
	ThreadData* data;
	for (list<ThreadData*>::iterator i = threadInfo.begin(); i != threadInfo.end(); ++i)
	{
		data = (*i);
		if (data->thread == thread)
		{
			threadInfo.remove(data);
			return;
		}
	}
}

DWORD WINAPI ThreadManager::ThreadProcess(LPVOID arg)
{
	//((SelectManager*)arg)->SelectProcess((void*)GetCurrentThreadId());
	return 1;
}

DWORD ThreadManager::CommunicationProcess(LPVOID arg)
{
	((IocpModel*)arg)->IocpProcess();
	return 1;
}

void* ThreadManager::getData(DWORD threadID)
{
	ThreadData* data;
	for (list<ThreadData*>::iterator i = threadInfo.begin(); i != threadInfo.end(); ++i)
	{
		data = (*i);
		if (GetThreadId(data->thread) == threadID)
		{
			return data->data;
		}
	}
	return nullptr;
}

bool ThreadManager::searchData(ThreadData*& data)
{
	if (save != threadInfo.end())
	{
		data = *save;
		++save;
		return true;
	}
	else
	{
		return false;
	}
}
void ThreadManager::startSearch()
{
	save = threadInfo.begin();
}