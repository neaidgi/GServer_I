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

	// 동기화
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

//// 교수님이 주신 동기화
//class CCriticalSection
//{
//public:
//	CCriticalSection(VOID)
//	{
//		InitializeCriticalSection(&mSync);
//	}
//
//	~CCriticalSection(VOID)
//	{
//		DeleteCriticalSection(&mSync);
//	}
//
//	inline VOID Enter(VOID)
//	{
//		EnterCriticalSection(&mSync);
//	}
//
//	inline VOID Leave(VOID)
//	{
//		LeaveCriticalSection(&mSync);
//	}
//
//private:
//	CRITICAL_SECTION	mSync;
//};
//
//
//
//template <class T>
//class CMultiThreadSync
//{
//	friend class CThreadSync;
//public:
//	class CThreadSync
//	{
//	public:
//		CThreadSync(VOID)
//		{
//			T::mSync.Enter();
//		}
//
//		~CThreadSync(VOID)
//		{
//			T::mSync.Leave();
//		}
//	};
//
//private:
//	static CCriticalSection mSync;
//};
//
//template <class T>
//
//CCriticalSection CMultiThreadSync<T>::mSync;

#endif