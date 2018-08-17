#ifndef _CLOSE_STATE_H_
#define _CLOSE_STATE_H_

#include"UserState.h"

class CloseState : public UserState
{
private:
	static CloseState* Instance;

	CloseState() {};
	~CloseState() {};
public:
	static void CreateInstance();
	static CloseState* GetInstance();
	static void DestroyInstance();

	virtual bool Read(User* _user);
	virtual bool Write(User* _user);
};

#endif
