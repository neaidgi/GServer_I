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
	char m_id[IDSIZE];
	char pw[IDSIZE];
	int channelnum;
	bool login;
	bool ingame;
	int partyroomnum;
	// ���� ����
	UserState* state;

	// Ÿ��Ʋ(�α���ȭ��) ����
	LoginState* loginstate;
	// ĳ���� ���� ȭ�� ����
	CharacterState* characterstate;
	// �ΰ��� ����
	InGameState* ingamestate;

	SlotData* characterslot[SLOTMAXCOUNT];
	Character* current_character;
	int slotcount;
	// ��Ƽ�����ΰ�
	bool is_partyleader;
	bool is_slotload;
	bool is_callback;
	// �����ȿ� �ֳ�
	bool is_dungeon;
	// ĳ���Ͱ� ����ֳ�
	bool is_Characterlive;
	// ����Ǿ��ֳ�
	bool is_gamereday;
public:
	User(SOCKET _sock, SOCKADDR_IN _addr);
	~User();

	// -- ���� ���� -- //
	UserState* getState();
	LoginState* getLoginState();
	CharacterState* getCharacterState();
	InGameState* getInGameState();
	// ��������
	void SetState(UserState* _state);

	// -- �ʱ�ȭ ���� -- //
	void ResetUserInfo();
	void ResetSlot() { for (int i = 0; i < slotcount; i++) delete characterslot[i]; memset(characterslot, 0, sizeof(characterslot)); }
	void ResetCurCharacter() { delete current_character; current_character = nullptr; }

	// -- ���� ĳ���� ���� -- //
	void SetCurCharacter(Character* _player);
	Character* GetCurCharacter() { return current_character; }
	bool DeleteCharacter(int _index);
	bool CurCharacter_HP_Down(int _damage);
	void SetCurCharacter_Live() { is_Characterlive = true; }
	void SetCurCharacter_Dead() { is_Characterlive = false; }
	bool GetCurCharacter_IsLive() {	return is_Characterlive; }

	// -- ���԰���(���Թ�ȣ,��������,�����ʱ�ȭ) -- //
	bool SetSlot(SlotData* _slotdata);
	const int GetSlotCount() { return slotcount; }
	void SetSlotCount(int _count) { slotcount = _count; }
	bool isSlotLoaded() { return is_slotload; }
	void SlotLoadComplete() { is_slotload = true; }

	// -- �α��� ���� -- //
	void setID(char *id);
	const char* getID();
	void setPW(char *pw);
	const char* getPW();
	// �α���,�α׾ƿ� ����������
	void setLogin() { login = true; }
	void setLogout() { login = false; }
	// �α����ߴ���
	bool isLogin() { return login ? true : false; }

	// -- ��Ƽ���� ���� -- //
	void SetPartyNum(int _partyroomnum) { partyroomnum = _partyroomnum; }
	int GetPartyNum() { return partyroomnum; }
	void SetPartyLeader(bool _leader) { is_partyleader = _leader; }
	bool isParty() { return partyroomnum == -1 ? false : true; }
	bool isPartyLeader() { return is_partyleader ? true : false; }
	void ResetPartyInfo() { partyroomnum = -1; is_partyleader = false; }

	//  -- ���� ���� -- //
	void SetEnterDungeon() { is_dungeon = true; }
	void SetLeaveDungeon() { is_dungeon = false; }
	bool isDungeon() { return is_dungeon; }
	// void ResetDungeonInfo() { is_dungeon = false; }
	bool GetiStageReady() { return is_gamereday; }
	void SetStageReady() { is_gamereday = true; }
	void SetNotStageReady() { is_gamereday = false; }

	// -- ä�� ���� -- //
	const int GetChannelNum() { return channelnum; }
	void SetChannelNum(int _channelnum) { channelnum = _channelnum; }
	bool isChannel() { return channelnum == -1 ? false : true; }
	
	// -- �ΰ��� ���� -- //
	void SetEnterGame() { ingame = true; }
	void SetLeaveGame() { ingame = false; }
	bool isIngame() { return ingame ? true : false; }

	// -- ��Ű��� -- //
	void SetCallback(bool _callback) { is_callback = _callback; }
	bool GetCallback() { return is_callback; }

	void InitState();
};

#endif
