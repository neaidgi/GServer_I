#include"InGameState.h"
#include "CharacterManager.h"

bool InGameState::Read(User * _user)
{
	CharacterManager* charactermanager = CharacterManager::GetInstance();
	int result;

	switch (state)
	{
	case INGAME_INIT_RECV:
		//charactermanager->
		break;
	default:
		break;
	}

	return true;
}

bool InGameState::Write(User * _user)
{
	switch (state)
	{
	case INGAME_INIT_SEND:
		break;
	default:
		break;
	}

	return true;
}
