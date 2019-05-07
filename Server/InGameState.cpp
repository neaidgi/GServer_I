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
			// ���� ��������
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_PARTY_INVITE)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_PARTY_INVITE_RESULT)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_PARTY_LEADER_DELEGATE_RESULT)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_DUNGEON_ENTER_RESULT)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_DUNGEON_LEAVE_RESULT)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_DUNGEON_MONSTER_INFO_RESULT)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGAME_DUNGEON_STAGE_IN_RESULT)
		{
			state = INGAME_INIT_RECV;
		}
		else if (result == RT_INGMAE_MONSTER_MOVE_RESULT)
		{
			state = INGAME_INIT_RECV;
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
