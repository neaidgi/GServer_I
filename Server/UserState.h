#ifndef _USER_STATE_H_
#define _USER_STATE_H_

#include "Global.h"

class User;
/*
	UserState
	- 유저의 상태에 맞는 작업을 하는 상태패턴의 가장 부모단계의 클래스
*/
class UserState
{
private:
public:
	UserState() {};
	~UserState() {};

	virtual bool Read(User* _user) = 0;
	virtual bool Write(User* _user) = 0;
};

#endif
