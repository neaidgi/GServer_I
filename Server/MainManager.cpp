#include "MainManager.h"
#include "LoginState.h"
#include "TenderState.h"
#include "IocpModel.h"
#include "GameManager.h"
#include "InGameManager.h"

MainManager* MainManager::Instance = nullptr;

MainManager::MainManager()
{

}
MainManager::~MainManager()
{

}

void MainManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new MainManager();
		CriticalSectionManager::CreateInstance();
		MsgManager::CreateInstance();
		LogManager::CreateInstance();
		ErrorManager::CreateInstance();
		UserManager::CreateInstance();
		DBManager::CreateInstance();
		ThreadManager::CreateInstance();
		LoginManager::CreateInstance();
		CharacterManager::CreateInstance();
		GameManager::CreateInstance();
		GameDataManager::CreateInstance();
		EncryptManager::CreateInstance(ENCRYPT_KEY);
		InGameManager::CreateInstance();
	}
}
MainManager* MainManager::GetInstance()
{
	return Instance;
}
void MainManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;

		InGameManager::DestroyInstance();
		GameDataManager::DestroyInstance();
		UserManager::DestroyInstance();
		ThreadManager::DestroyInstance();
		LoginManager::DestroyInstance();
		CharacterManager::DestroyInstance();
		EncryptManager::DestroyInstance();
		GameManager::DestroyInstance();
		DBManager::DestroyInstance();
		CriticalSectionManager::DestroyInstance();
		LogManager::DestroyInstance();
		ErrorManager::DestroyInstance();
		MsgManager::DestroyInstance();
	}
}
BOOL MainManager::CtrlHandle(DWORD _ctrl)
{
	switch (_ctrl)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_BREAK_EVENT:

		MainManager::GetInstance()->EndManager();
		MainManager::GetInstance()->DestroyInstance();
		break;
	}
	return FALSE;
}
bool MainManager::Acceptprocess()
{
	while (1)
	{
		SOCKET sock = server->acceptClient();
		if (sock == INVALID_SOCKET)
		{
			return false;
		}
		else
		{
			// IO ��Ʈ ����
			GameManager::GetInstance()->PortBind(sock);
		
			// ���� ����
			User* user = UserManager::GetInstance()->addUser(sock);

			// ������ ����ü ���� �� ACCEPT �Ϸ� ���� ����
			WSAOVERLAPPEDEx wsaoverlapped;
			wsaoverlapped.p = user;
			wsaoverlapped.type = IOTYPE_ACCEPT;
			PostQueuedCompletionStatus(GameManager::GetInstance()->GetIOCPHandle(), 1, sock, &wsaoverlapped.overlapped);
		}
	}
	return true;
}
void MainManager::Run()
{
	if (Initialize() == false)
	{
		ErrorManager::GetInstance()->err_quit("WSA Init fail");
	}

	if (MangerInitialize() == false)
	{
		ErrorManager::GetInstance()->err_quit("Manager Init fail");
	}
	
	server = new TCPServer();

	if (server->start(SERVERPORT) == false)
	{
		ErrorManager::GetInstance()->err_quit("���� : TCP���� ���� ����");
	}
	MsgManager::GetInstance()->DisplayMsg("����","TCP���� ���� ����");
	// while 
	Acceptprocess();

	EndManager();
}

bool MainManager::Initialize()
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)	// ���� DLL �ʱ�ȭ(DLL�� �޸𸮿� �ø�). ����ϱ⿡ �ռ� "ws2_32"���̺귯���� ���Խ��������.( #pragma comment(lib, "ws2_32")  )
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("MainManager::Initialize : ERROR : WSAStartup() failed ");
		return false;
	}
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandle, TRUE);

	return true;
}

bool MainManager::MangerInitialize()
{
	MsgManager::GetInstance()->InitializeManager();
	LogManager::GetInstance()->InitializeManager();
	ErrorManager::GetInstance()->InitializeManager();

	CriticalSectionManager::GetInstance()->InitializeManager();

	MsgManager::GetInstance()->DisplayMsg("����","�����ͺ��̽� �ʱ�ȭ��");
	// DB ����
	if (DBManager::GetInstance()->InitializeDB() == false)
	{
		ErrorManager::GetInstance()->err_display("�����ͺ��̽� �ʱ�ȭ ����");
		return false;
	}
	MsgManager::GetInstance()->DisplayMsg("����","�����ͺ��̽� �ʱ�ȭ ����");
	UserManager::GetInstance()->InitializeManager();
	ThreadManager::GetInstance()->InitializeManager();
	LoginManager::GetInstance()->InitializeManager();
	if (CharacterManager::GetInstance()->InitializeManager() == false)
	{
		//�α�
		ErrorManager::GetInstance()->err_quit("CharacterManager Init fail");
	}
	EncryptManager::GetInstance()->InitializeManager();
	MsgManager::GetInstance()->DisplayMsg("����", "IOCP �ʱ�ȭ ��");
	if (GameManager::GetInstance()->InitializeManager() == false)
	{
		ErrorManager::GetInstance()->err_quit("IOCP Init fail");

		return false;
	}
	MsgManager::GetInstance()->DisplayMsg("����", "IOCP �ʱ�ȭ ����");
	if (GameDataManager::GetInstance()->InitializeManager() == false)
	{
		ErrorManager::GetInstance()->err_quit("���� ������ �ε� ����");
		return false;
	}
	InGameManager::GetInstance()->MangerInitialize();
	return true;
}

// ���� ����
void MainManager::EndManager()
{
	server->stop();
	delete server;

	// �� �Ŵ����� End ȣ��
	InGameManager::GetInstance()->EndManager();
	GameDataManager::GetInstance()->EndManager();
	LogManager::GetInstance()->EndManager();
	UserManager::GetInstance()->EndManager();
	ThreadManager::GetInstance()->EndManager();
	//TenderManager::GetInstance()->EndManager();
	LoginManager::GetInstance()->EndManager();
	CharacterManager::GetInstance()->EndManager();
	ErrorManager::GetInstance()->EndManager();
	EncryptManager::GetInstance()->EndManager();
	GameManager::GetInstance()->EndManager();
	// DB ���� ����
	DBManager::GetInstance()->EndManager();
	CriticalSectionManager::GetInstance()->EndManager();
	MsgManager::GetInstance()->EndManager();

	// ���� ����
	WSACleanup();	// ���� DLL �޸� ����
}
