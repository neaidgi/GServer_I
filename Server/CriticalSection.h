#ifndef _CRITICALSECTION_H_
#define _CRITICALSECTION_H_

#include "Global.h"
#include "MsgManager.h"

/*
	CCriticalSection
	 - 생성자(열쇠 생성)
	 - 소멸자(열쇠 삭제)
	 - Enter(열쇠 가져오기)
	 - Leave(열쇠 반납)

	CMultiThreadSync
	 - 이 클래스를 상속 받고 class CThreadSync를 변수로 선언하면 생성자에서 열쇠를 가져오고 소멸자에서 열쇠를 반납한다

*/
class CCriticalSection
{
public:
	CCriticalSection(VOID)
	{
		InitializeCriticalSection(&mSync);
	}

	~CCriticalSection(VOID)
	{
		DeleteCriticalSection(&mSync);
	}

	inline VOID Enter(VOID)
	{
		EnterCriticalSection(&mSync);
	}

	inline VOID Leave(VOID)
	{
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
			T::mSync.Enter();
		}

		~CThreadSync(VOID)
		{
			T::mSync.Leave();
		}
	};

private:
	static CCriticalSection mSync;
};

template <class T>
CCriticalSection CMultiThreadSync<T>::mSync;

#endif