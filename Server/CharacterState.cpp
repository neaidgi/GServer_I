#include "CharacterState.h"
#include "CharacterManager.h"

bool CharacterState::Read(User * _user)
{
	CharacterManager* charactermanager = CharacterManager::GetInstance();
	int result;

	switch (state)
	{
	case CharacterState::CHARACTER_MENU_RECV:
		result = charactermanager->Character_Init_Choice(_user);
		if (result == RT_CHARACTER_SLOTRESULT)
		{
			state = CHARACTER_SLOT_RESULT_SEND;
		}
		else if (result == RT_CHARACTER_ENTERCREATE)
		{
			state = CHARACTER_CREATE_MENU_SEND;
		}
		else if (result == RT_CHARACTER_ENTERGAME)
		{
			state = CHARACTER_ENTER_SEND;
		}
		else if (result == RT_CHARACTER_DELETE)
		{
			state = CHARACTER_DELETE_SEND;
		}
		else if (result == RT_CHARACTER_ENTERGAMEFAIL)
		{
			state = CHARACTER_ENTERFAIL_SEND;
		}
		break;
	case CharacterState::CHARACTER_REQ_RECV:
		result = charactermanager->Character_Management_Process(_user);
		if (result == RT_CHARACTER_NICKOVERLAP_TRUE)
		{
			state = CHARACTER_REQ_OVLAP_NICK_SEND;
		}
		else if (result == RT_CHARACTER_CREATE)
		{
			state = CHARACTER_REQ_CHARACTER_SEND;
		}
		else if (result == RT_CHARACTER_EXIT)
		{
			state = CHARACTER_EXIT_SEND;
		}
	default:
		break;
	}

	return true;
}

bool CharacterState::Write(User * _user)
{
	switch (state)
	{
	case CharacterState::CHARACTER_SLOT_RESULT_SEND:
		state = CHARACTER_MENU_RECV;
		break;
	case CharacterState::CHARACTER_CREATE_MENU_SEND:
		state = CHARACTER_REQ_RECV;
		break;
	case CharacterState::CHARACTER_ENTER_SEND:
		state = CHARACTER_MENU_RECV;
		_user->SetState(_user->getInGameState());
		break;
	case CharacterState::CHARACTER_ENTERFAIL_SEND:
		state = CHARACTER_MENU_RECV;
		break;
	case CharacterState::CHARACTER_REQ_OVLAP_NICK_SEND:
		state = CHARACTER_REQ_RECV;
		break;
	case CharacterState::CHARACTER_REQ_CHARACTER_SEND:
		state = CHARACTER_MENU_RECV;
		break;
	case CharacterState::CHARACTER_EXIT_SEND:
		state = CHARACTER_MENU_RECV;
		break;
	case CharacterState::CHARACTER_DELETE_SEND:
		state = CHARACTER_MENU_RECV;
		break;
	default:
		break;
	}

	return true;
}
