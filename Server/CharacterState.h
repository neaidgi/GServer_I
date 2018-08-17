#ifndef _CHARACTER_STATE_H_
#define _CHARACTER_STATE_H_

#include"UserState.h"

class CharacterState :public UserState
{
private:
	enum CSTATE {
		// RECV 스테이트
		INIT_RECV, REQUEST_RECV, LOGIN_RECV,
		// SEND 스테이트
		INIT_SEND, REQUEST_RESULT_SEND, LOGIN_RESULT_SEND, LOGINFAIL_RESULT_SEND, LOGOUT_SEND, LEAVE_SEND, JOIN_EXIT_SEND,
		TENDER_SEND
	};

	CSTATE state;
public:

	CharacterState() { state = INIT_RECV; };
	~CharacterState() {};

	virtual bool Read(User* _user);
	virtual bool Write(User* _user);
};

#endif
