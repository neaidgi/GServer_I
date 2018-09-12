#ifndef _INGAME_STATE_H_
#define _INGAME_STATE_H_

#include"UserState.h"

class InGameState :public UserState
{
private:
	enum CSTATE {
		// RECV ������Ʈ
		INGAME_INIT_RECV,
		// SEND ������Ʈ
		INGAME_INIT_SEND
	};

	CSTATE state;
public:

	InGameState() { state = INGAME_INIT_RECV; };
	~InGameState() {};

	virtual bool Read(User* _user);
	virtual bool Write(User* _user);
};

#endif
