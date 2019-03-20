#ifndef USER_H
#define USER_H

#include "Packet.h"
#include "UserState.h"
#include "LoginState.h"
#include "CharacterState.h"
#include "InGameState.h"
#include "Character.h"

#define SLOTMAXCOUNT 3 

struct SlotData 
{
	int jobcode;
	char* jobname;
	char* nick;
	int level;
	char* code;

	SlotData() 
	{
		code = nullptr;
		jobname = nullptr;
		nick = nullptr;
	}
	~SlotData() 
	{
		if (jobname != nullptr)
			delete[] jobname;
		if (nick != nullptr)
			delete[] nick;
		if (code != nullptr)
			delete[] code;
	}
};


class User : public Packet
{
private:
	char id[IDSIZE];
	char pw[IDSIZE];
	int channelnum;
	bool login;
	bool ingame;
	int partyroomnum;
	UserState* state;

	LoginState* loginstate;
	CharacterState* characterstate;
	InGameState* ingamestate;

	SlotData* characterslot[SLOTMAXCOUNT];
	Character* current_character;
	int slotcount;
	bool is_partyleader;
	bool is_slotload;
	bool is_callback;
public:
	User(SOCKET _sock, SOCKADDR_IN _addr);
	~User();
	UserState* getState();
	LoginState* getLoginState();
	CharacterState* getCharacterState();
	InGameState* getInGameState();
	void SetCurCharacter(Character* _player);
	Character* GetCurCharacter() { return current_character; }
	bool SetSlot(SlotData* _slotdata);
	const int GetChannelNum() { return channelnum; }
	void SetChannelNum(int _channelnum) { channelnum = _channelnum; }
	const int GetSlotCount() { return slotcount; }
	void SetSlotCount(int _count) { slotcount = _count; }
	void ResetSlot() { for (int i = 0; i < slotcount; i++) delete characterslot[i]; memset(characterslot, 0, sizeof(characterslot)); }
	void ResetCurCharacter() { delete current_character; current_character = nullptr; }
	void ResetUserInfo();
	bool DeleteCharacter(int _index);
	bool isSlotLoaded() { return is_slotload; }
	void SlotLoadComplete() { is_slotload = true; }
	void SetState(UserState* _state);
	void setID(char *id);
	const char* getID();
	void setPW(char *pw);
	const char* getPW();
	void setLogin() { login = true; }
	void setLogout() { login = false; }
	void SetEnterGame() { ingame = true; }
	void SetLeaveGame() { ingame = false; }
	void SetPartyNum(int _partyroomnum) { partyroomnum = _partyroomnum; }
	int GetPartyNum() { return partyroomnum; }
	void SetCallback(bool _callback) { is_callback = _callback; }
	bool GetCallback() { return is_callback; }
	bool isLogin() { return login ? true : false; }
	bool isIngame() { return ingame ? true : false; }
	bool isParty() { return partyroomnum == -1 ? true : false; }

	void InitState();
};

#endif
