#include "TenderState.h"
#include "TenderSystem.h"
#include "UserManager.h"
#include "MainManager.h"

bool TenderState::Read(User* _user)
{
	TenderManager* tendermanager = TenderManager::GetInstance();
	int result;
	
	//if (_user->recvMsg() == false)
	//{
	//	return false;
	//}
	//if (_user->isRecvSuccess() == false)
	//{
	//	return true;
	//}

	switch (state)
	{
	case TenderState::TENDERMENU_RECV:
		result = tendermanager->tenderMenuChoice(_user);
		if (result == RT_MENUCREATE)
		{
			state = TENDER_SEND;
		}
		else if (result == RT_MENUPARTICIPATE)
		{
			state = TENDER_SEND;
		}
		else if (result == RT_MENUBACK)
		{
			state = LOGINMENU_SEND;
		}
		break;
	case TenderState::TENDER_RECV:
		result = tendermanager->TenderRecvProcess(_user);
		if (result == RT_TENDERCREATE)
		{
			state = TENDER_RESULT_SEND;
		}
		else if (result == RT_TENDERPARTICIPATE)
		{
			state = TENDER_RESULT_SEND;
		}
		else if (result == RT_TENDERLIST)
		{
			state = TENDER_RESULT_SEND;
		}
		break;

	default:
		break;
	}
	return true;
}

bool TenderState::Write(User* _user)
{
	TenderManager* tendermanager = TenderManager::GetInstance();

	//if (_user->sendMsg() == false)
	//{
	//	return false;
	//}

	//if (_user->isSendSuccess())
	//{
	//	_user->include_wset = false;
	//}

	switch (state)
	{
	case TenderState::TENDER_SEND:
		state = TENDER_RECV;
		break;
	case TenderState::TENDER_RESULT_SEND:
		state = TENDERMENU_RECV;
		break;
	case TenderState::LOGINMENU_SEND:
		state = TENDERMENU_RECV;		// Àü ´Ü°è
		_user->SetState(_user->getLoginState());
		break;
	default:
		break;
	}
	return true;
}
