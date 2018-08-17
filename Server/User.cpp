#include "User.h"
#include "LoginState.h"
// »ý¼ºÀÚ
User::User(SOCKET _sock, SOCKADDR_IN _addr): Packet(_sock, _addr)
{
	ZeroMemory(id, IDSIZE);
	ZeroMemory(pw, IDSIZE);
	money = 100000;

	state = nullptr;
}

User::~User()
{
	delete loginstate;
	delete tenderstate;
}

UserState* User::getState()
{
	return state;
}

LoginState * User::getLoginState()
{
	return loginstate;
}

TenderState * User::getTenderState()
{
	return tenderstate;
}

void User::SetState(UserState * _state)
{
	state = _state;
}


void User::setID(char * id)
{
	memcpy(this->id, id, sizeof(this->id));
}

const char * User::getID()
{
	return id;
}

void User::setPW(char * pw)
{
	memcpy(this->pw, pw, sizeof(this->pw));
}

const char * User::getPW()
{
	return pw;
}

bool User::checkmoney(int money)
{
	return true;
}

void User::setmoney(int money)
{

}

void User::InitState()
{
	loginstate = new LoginState();
	tenderstate = new TenderState();
	state = loginstate;
}
