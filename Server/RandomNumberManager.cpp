#include "RandomNumberManager.h"

RandomNumberManager* RandomNumberManager::Instance = nullptr;

RandomNumberManager::RandomNumberManager()
{

}

RandomNumberManager::~RandomNumberManager()
{
}

void RandomNumberManager::CreateInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}

	Instance = new RandomNumberManager();
}

RandomNumberManager * RandomNumberManager::GetInstance()
{
	return Instance;
}

void RandomNumberManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool RandomNumberManager::InitializeManager()
{
	// seed �� �ʱ�ȭ
	srand((unsigned int)time(NULL));
	return true;
}

void RandomNumberManager::EndManager()
{
}
