#pragma once
#ifndef _TENDER_STATE_H
#define _TENDER_STATE_H

#include"UserState.h"

class TenderState : public UserState
{
private:
	enum TSTATE {
		// RECV 스테이트
		TENDERMENU_RECV, TENDER_RECV,
		// SEND 스테이트
		TENDER_SEND, TENDER_RESULT_SEND, LOGINMENU_SEND
	};

	TSTATE state;
public:
	TenderState() { state = TENDERMENU_RECV; };
	~TenderState() {};

	virtual bool Read(User* _user);
	virtual bool Write(User* _user);
};

#endif
