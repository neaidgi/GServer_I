#ifndef MAIN_MANAGER_H
#define MAIN_MANAGER_H

#include "UserManager.h"
#include "TCPServer.h"
#include "ThreadManager.h"
#include "ErrorManager.h"
#include "LoginManager.h"
#include "LogManager.h"
#include "DBManager.h"
#include "CharacterManager.h"
#include "GameDataManager.h"
#include "MsgManager.h"
#include "CriticalSection.h"

/*
	[MainManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

class MainManager
{
private:
	static MainManager* Instance;
	TCPServer* server;

	MainManager();
	~MainManager();
public:
	static void CreateInstance();
	static MainManager* GetInstance();
	static void DestroyInstance();

	static BOOL CtrlHandle(DWORD _ctrl);

	bool Acceptprocess();
	void Run();														// 메인
	bool Initialize();											    // Wsa 초기화
	bool MangerInitialize();
	void EndManager();

};

#endif
