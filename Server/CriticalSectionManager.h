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

#endif