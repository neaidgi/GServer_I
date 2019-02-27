#include"CriticalSectionManager.h"

CriticalSectionManager* CriticalSectionManager::Instance = nullptr;

CriticalSectionManager::CriticalSectionManager()
{

}

CriticalSectionManager::~CriticalSectionManager()
{

}

void CriticalSectionManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new CriticalSectionManager();
	}
}

CriticalSectionManager * CriticalSectionManager::GetInstance()
{
	return Instance;
}

void CriticalSectionManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool CriticalSectionManager::InitializeManager()
{
	InitializeCriticalSection(&cs);
	return true;
}

void CriticalSectionManager::EndManager()
{
	DeleteCriticalSection(&cs);
}

void CriticalSectionManager::Enter()
{
	EnterCriticalSection(&cs);
}

void CriticalSectionManager::Leave()
{
	LeaveCriticalSection(&cs);
}
