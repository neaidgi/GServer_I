#ifndef USER_H
#define USER_H

#include "Packet.h"
#include "UserState.h"
#include "LoginState.h"
#include "TenderState.h"

class User : public Packet
{
private:
	char id[IDSIZE];
	char pw[IDSIZE];
	int money;
	bool login;
	UserState* state;

	LoginState* loginstate;
	TenderState* tenderstate;

public:
	User(SOCKET _sock, SOCKADDR_IN _addr);
	~User();
	UserState* getState();
	LoginState* getLoginState();
	TenderState* getTenderState();
	void SetState(UserState* _state);
	void setID(char *id);
	const char* getID();
	void setPW(char *pw);
	const char* getPW();
	bool checkmoney(int money);
	void setmoney(int money);
	void setLogin() { login = true; }
	void setLogout() { login = false; }
	bool isLogin() { return login ? true : false; }

	void InitState();
};

#endif
