#ifndef _CRITICALSECTION_MANAGER_H_
#define _CRITICALSECTION_MANAGER_H_

#include "Global.h"
#include "MsgManager.h"

class CriticalSectionManager
{
private:
	static CriticalSectionManager* Instance;

	CriticalSectionManager();
	~CriticalSectionManager();

	// µø±‚»≠
	CRITICAL_SECTION cs;

public:
	static void CreateInstance();
	static CriticalSectionManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void Enter();
	void Leave();

};

class CCriticalSection
{
public:
	CCriticalSection(VOID)
	{
		InitializeCriticalSection(&mSync);
		printf(" [InitializeCriticalSection] \n");
	}

	~CCriticalSection(VOID)
	{
		DeleteCriticalSection(&mSync);
		printf(" [InitializeCriticalSection] \n");
	}

	inline VOID Enter(VOID)
	{
		printf(" [Enter] \n");
		EnterCriticalSection(&mSync);
	}

	inline VOID Leave(VOID)
	{
		printf(" [Leave] \n");
		LeaveCriticalSection(&mSync);
	}

private:
	CRITICAL_SECTION	mSync;
};

template <class T>
class CMultiThreadSync
{
	friend class CThreadSync; 
public:
	class CThreadSync
	{
	public:
		CThreadSync(VOID)
		{
			printf(" [Enter] \n");
			T::mSync.Enter();
		}

		~CThreadSync(VOID)
		{
			printf(" [Leave] \n");
			T::mSync.Leave();
		}
	};

private:
	static CCriticalSection mSync;
};

template <class T>
CCriticalSection CMultiThreadSync<T>::mSync;

#endif