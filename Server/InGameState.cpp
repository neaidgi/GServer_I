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
		if (result == RT_INGAME_OTHERPLAYER_LIST)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_MOVE)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_CHANNEL_INFO)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_CHANNEL_CHANGE)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_MENU_CHARACTER)
		{
			state = INGAME_INIT_RECV;
			// ���� ���� ����(ĳ���ͼ���ȭ��)
			_user->SetState(_user->getCharacterState());
		}
		else if (result == RT_INGAME_MENU_LOGOUT)
		{
			state = INGAME_INIT_RECV;
			// ���� ���� ����(Ÿ��Ʋ)
			_user->SetState(_user->getLoginState());
		}
		else if (result == RT_INGAME_MENU_EXIT)
		{

		}
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
