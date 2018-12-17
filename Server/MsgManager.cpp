#include "MsgManager.h"

MsgManager* MsgManager::Instance = nullptr;

MsgManager::MsgManager()
{
	
}

MsgManager::~MsgManager()
{

}

void MsgManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new MsgManager();
	}
}

MsgManager* MsgManager::GetInstance()
{
	return Instance;
}

void MsgManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool MsgManager::InitializeManager()
{
	return true;
}

void MsgManager::EndManager()
{
}

void MsgManager::DisplayMsg(char* _type, char * _msg)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	char buf[12];

	sprintf(buf, "[%d:%d:%d]", time.wHour, time.wMinute, time.wSecond);
	printf("%s :: %s : %s\n", buf, _type, _msg);
}
