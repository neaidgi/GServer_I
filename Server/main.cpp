#include "MainManager.h"

int main()
{
	MainManager::CreateInstance();
	MainManager::GetInstance()->Run();
	MainManager::DestroyInstance();
	
	return 0;
}