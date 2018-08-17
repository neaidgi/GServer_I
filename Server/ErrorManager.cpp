#include"ErrorManager.h"

ErrorManager* ErrorManager::Instance = nullptr;

ErrorManager::ErrorManager()
{
}

ErrorManager::~ErrorManager()
{
}

void ErrorManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new ErrorManager();
	}
}

ErrorManager * ErrorManager::GetInstance()
{
	return Instance;
}

void ErrorManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool ErrorManager::InitializeManager()
{
	return true;
}

void ErrorManager::EndManager()
{

}

void ErrorManager::err_quit(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void ErrorManager::err_display(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s]%d %s\n", msg, WSAGetLastError(), (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

