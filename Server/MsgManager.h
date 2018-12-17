#ifndef _MSG_MANAGER_H_
#define _MSG_MANAGER_H_

#include "Global.h"

class MsgManager{
private:
	static MsgManager* Instance;

	MsgManager();
	~MsgManager();

public:
	static void CreateInstance();
	static MsgManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void DisplayMsg(char* _type, char* _msg);
};

#endif