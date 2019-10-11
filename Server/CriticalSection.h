#ifndef _CRITICALSECTION_H_
#define _CRITICALSECTION_H_

#include "Global.h"
#include "MsgManager.h"

/*
	CCriticalSection
	 - ������(���� ����)
	 - �Ҹ���(���� ����)
	 - Enter(���� ��������)
	 - Leave(���� �ݳ�)

	CMultiThreadSync
	 - �� Ŭ������ ��� �ް� class CThreadSync�� ������ �����ϸ� �����ڿ��� ���踦 �������� �Ҹ��ڿ��� ���踦 �ݳ��Ѵ�

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