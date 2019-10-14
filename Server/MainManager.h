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
	
	CreateInstance() - �Ŵ��� ����
	GetInstance() - �Ŵ��� ����������
	DestroyInstance() - �Ŵ��� ����
	
	InitializeManager() - �ʱ�ȭ
	EndManager() - ������
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
	void Run();														// ����
	bool Initialize();											    // Wsa �ʱ�ȭ
	bool MangerInitialize();
	void EndManager();

};

#endif
