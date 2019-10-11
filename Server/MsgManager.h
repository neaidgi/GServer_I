#ifndef _MSG_MANAGER_H_
#define _MSG_MANAGER_H_

#include "Global.h"

/*
	[MsgManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

class MsgManager
{
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