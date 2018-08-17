#include "CloseState.h"

CloseState* CloseState::Instance = nullptr;

void CloseState::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new CloseState();
	}
}
CloseState* CloseState::GetInstance()
{
	return Instance;
}
void CloseState::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool CloseState::Read(User * _user)
{
	return false;
}

bool CloseState::Write(User * _user)
{
	return false;
}
