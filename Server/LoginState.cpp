#include "LoginState.h"
#include "LoginManager.h"


bool LoginState::Read(User* _user)
{
	LoginManager* loginmanager = LoginManager::GetInstance();
	int result;
	
	// 세부 스테이트에 따라 다른 동작
	switch (state)
	{
	case LoginState::INIT_RECV:
		result = loginmanager->logoutMenuChoice(_user);
		if (result == RT_LOGINMENU)
		{
			state = INIT_SEND;
		}
		// 수정했음
		else if (result == RT_LOGIN)
		{
			state = LOGIN_RESULT_SEND;
		}
		// 수정했음
		else if (result == RT_LOGINFAIL)
		{
			state = LOGINFAIL_RESULT_SEND;
		}
		else if (result == RT_JOINMENU)
		{
			state = REQUEST_RESULT_SEND;
		}
		else if (result == RT_USER_DISCONNECT)
		{
			return false;
		}
		break;
	case LoginState::REQUEST_RECV:
		result = loginmanager->loginProcess(_user);
		if (result == RT_ID_OVERLAP)
		{
			state = REQUEST_RESULT_SEND;
		}
		else if (result == RT_JOIN)
		{
			state = REQUEST_RESULT_SEND;
		}
		else if (result = RT_EXIT_JOIN)
		{
			state = JOIN_EXIT_SEND;
		}
		break;
	case LoginState::LOGIN_RECV:
		result = loginmanager->loginMenuChoice(_user);
		if (result == RT_TENDER)
		{
			state = TENDER_SEND;
		}
		else if (result == RT_MEMBER_LEAVE)
		{
			state = LEAVE_SEND;
		}
		else if (result == RT_LOGOUT)
		{
			state = LOGOUT_SEND;
		}
		else if (result == RT_USER_DISCONNECT)
		{
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

bool LoginState::Write(User* _user)
{
	switch (state)
	{
	case LoginState::INIT_SEND:
		// 수정했음
		state = INIT_RECV;
		break;
	case LoginState::REQUEST_RESULT_SEND:
		state = REQUEST_RECV;
		break;
	case LoginState::JOIN_EXIT_SEND:
		state = INIT_RECV;
		break;
	case LoginState::LOGIN_RESULT_SEND:
		state = INIT_RECV;
		_user->SetState(_user->getCharacterState());
		break;
	case LoginState::LOGINFAIL_RESULT_SEND:
		state = INIT_RECV;
		break;
	case LoginState::TENDER_SEND:
		state = LOGIN_RECV;		// 전 단계
		_user->SetState(_user->getTenderState());
		break;
	case LoginState::LEAVE_SEND:
		state = INIT_RECV;
		break;
	case LoginState::LOGOUT_SEND:
		state = INIT_RECV;	
		break;
	default:
		break;
	}
	return true;
}
