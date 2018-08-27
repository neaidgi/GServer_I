#include "CharacterManager.h"

CharacterManager* CharacterManager::Instance = nullptr;

CharacterManager::CharacterManager()
{
}

CharacterManager::~CharacterManager()
{
}

void CharacterManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new CharacterManager();
	}
}

CharacterManager* CharacterManager::GetInstance()
{
	return Instance;
}

void CharacterManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool CharacterManager::InitializeManager()
{
	
	return true;
}

void CharacterManager::EndManager()
{

}