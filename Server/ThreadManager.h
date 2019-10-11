#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H
#include "Global.h"
//#include "MainManager.h"
#include <list>
using namespace std;

struct ThreadData
{
	HANDLE thread;
	void* data;
};

/*
	[ThreadManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

class ThreadManager
{

private:
	static ThreadManager* Instance;
	list<ThreadData*> threadInfo;
	list<ThreadData*>::iterator save;

	ThreadManager();
	~ThreadManager();

public:
	static void CreateInstance();
	static ThreadManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	HANDLE addThread(LPTHREAD_START_ROUTINE routine, LPVOID data, LPVOID _pthis);
	void removeThread(HANDLE thread);
	static DWORD WINAPI ThreadProcess(LPVOID arg);
	//static DWORD WINAPI TenderProcess(LPVOID arg);
	static DWORD WINAPI CommunicationProcess(LPVOID arg);
	//static DWORD WINAPI MonsterSpawnTimerProcess(LPVOID arg);
	void* getData(DWORD threadID);
	void startSearch();
	bool searchData(ThreadData*& data);

};

#endif
