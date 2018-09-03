#include "User.h"
#include "LoginState.h"


// ������
User::User(SOCKET _sock, SOCKADDR_IN _addr): Packet(_sock, _addr)
{
	ZeroMemory(id, IDSIZE);
	ZeroMemory(pw, IDSIZE);
	money = 100000;

	state = nullptr;
	memset(characterslot, 0, sizeof(characterslot));
	slotcount = 0;
	is_slotload = false;
}

User::~User()
{
	delete loginstate;
	delete tenderstate;

	for (int i = 0; i < slotcount; i++)
	{
		delete characterslot[i];
	}
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

CharacterState * User::getCharacterState()
{
	return characterstate;
}

// ĳ���� ���� �� ������ ĳ���� ����
void User::SetCurCharacter(int _index)
{
	for (int i = 0; slotcount > i; i++)
	{
		// ���� ����
		//if (characterslot[i]->code == _character_code)
		//{
		//	// ���� ����
		//}
	}
}

bool User::SetSlot(SlotData* _slotdata)
{
	if (slotcount == SLOTMAXCOUNT)
	{
		return false;
	}

	characterslot[slotcount++] = _slotdata;
	return true;
}

// ���� ĳ���� 1�� ���� // �� ��°���� _index �޾Ƽ� ���� �� �����
bool User::DeleteCharacter(int _index)
{
	if (slotcount == 0)
	{
		return false;
	}
	else if (characterslot[_index] == nullptr)
	{
		return false;
	}
	else
	{
		delete characterslot[_index];
		for (int i = _index; slotcount-1 > i; i++)
		{
			characterslot[i] = characterslot[i + 1];
		}

		characterslot[slotcount--] = nullptr;
		return true;
	}
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
	characterstate = new CharacterState();
	state = loginstate;
}
