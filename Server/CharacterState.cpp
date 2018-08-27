#include "CharacterState.h"

bool CharacterState::Read(User * _user)
{
	switch (state)
	{
	case INIT_RECV:
		//result = loginmanager->logoutMenuChoice(_user);
		//if (result == RT_LOGINMENU)
		//{
		//	state = INIT_SEND;
		//}
		//// 수정했음
		//else if (result == RT_LOGIN)
		//{
		//	state = LOGIN_RESULT_SEND;
		//}
		//// 수정했음
		//else if (result == RT_LOGINFAIL)
		//{
		//	state = LOGINFAIL_RESULT_SEND;
		//}
		//else if (result == RT_JOINMENU)
		//{
		//	state = REQUEST_RESULT_SEND;
		//}
		//else if (result == RT_USER_DISCONNECT)
		//{
		//	return false;
		//}
		break;
	default:
		break;
	}

	return true;
}

bool CharacterState::Write(User * _user)
{
	switch (state)
	{
	default:
		break;
	}

	return true;
}
