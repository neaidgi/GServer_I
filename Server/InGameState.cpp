#include"InGameState.h"
#include "InGameManager.h"

bool InGameState::Read(User * _user)
{
	InGameManager* ingamemanager = InGameManager::GetInstance();
	int result;

	switch (state)
	{
	case INGAME_INIT_RECV:
		result = ingamemanager->InGame_Init_Packet(_user);
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
