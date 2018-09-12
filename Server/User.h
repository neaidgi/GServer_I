#ifndef USER_H
#define USER_H

#include "Packet.h"
#include "UserState.h"
#include "LoginState.h"
#include "TenderState.h"
#include "CharacterState.h"
#include "InGameState.h"
#include "Character.h"

#define SLOTMAXCOUNT 3 

struct SlotData {
	int origincode;
	char* jobname;
	char* nick;
	int level;
	int code;

	SlotData() {
		jobname = nullptr;
		nick = nullptr;
	}
	~SlotData() {
		if (jobname != nullptr)
			delete jobname;
		if (nick != nullptr)
			delete nick;
	}
};


class User : public Packet
{
private:
	char id[IDSIZE];
	char pw[IDSIZE];
	int money;
	bool login;
	bool ingame;
	UserState* state;

	LoginState* loginstate;
	TenderState* tenderstate;
	CharacterState* characterstate;
	InGameState* ingamestate;

	SlotData* characterslot[SLOTMAXCOUNT];
	Character* current_character;
	int slotcount;
	bool is_slotload;
	bool is_callback;
public:
	User(SOCKET _sock, SOCKADDR_IN _addr);
	~User();
	UserState* getState();
	LoginState* getLoginState();
	TenderState* getTenderState();
	CharacterState* getCharacterState();
	InGameState* getInGameState();
	void SetCurCharacter(Character* _player);
	Character* GetCurCharacter() { return current_character; }
	bool SetSlot(SlotData* _slotdata);
	const SlotData* GetSlot(int _index) { return characterslot[_index - 1]; }		// 슬롯데이터 접근지정자
	const int GetSlotCount() { return slotcount; }
	bool DeleteCharacter(int _index);
	bool isSlotLoaded() { return is_slotload; }
	void SlotLoadComplete() { is_slotload = true; }
	void SetState(UserState* _state);
	void setID(char *id);
	const char* getID();
	void setPW(char *pw);
	const char* getPW();
	bool checkmoney(int money);
	void setmoney(int money);
	void setLogin() { login = true; }
	void setLogout() { login = false; }
	void SetEnterGame() { ingame = true; }
	void SetLeaveGame() { ingame = false; }
	void SetCallback(bool _callback) { is_callback = _callback; }
	bool GetCallback() { return is_callback; }
	bool isLogin() { return login ? true : false; }
	bool isIngame() { return ingame ? true : false; }

	void InitState();
};

#endif
