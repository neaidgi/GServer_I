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
	// 유저 상태
	UserState* state;

	// 타이틀(로그인화면) 상태
	LoginState* loginstate;
	// 캐릭터 선택 화면 상태
	CharacterState* characterstate;
	// 인게임 상태
	InGameState* ingamestate;

	SlotData* characterslot[SLOTMAXCOUNT];
	Character* current_character;
	int slotcount;
	// 파티리더인가
	bool is_partyleader;
	bool is_slotload;
	bool is_callback;
	// 던전안에 있나
	bool is_dungeon;
	// 캐릭터가 살아있나
	bool is_Characterlive;
	// 레디되어있나
	bool is_gamereday;
public:
	User(SOCKET _sock, SOCKADDR_IN _addr);
	~User();

	// -- 상태 관련 -- //
	UserState* getState();
	LoginState* getLoginState();
	CharacterState* getCharacterState();
	InGameState* getInGameState();
	// 상태저장
	void SetState(UserState* _state);

	// -- 초기화 관련 -- //
	void ResetUserInfo();
	void ResetSlot() { for (int i = 0; i < slotcount; i++) delete characterslot[i]; memset(characterslot, 0, sizeof(characterslot)); }
	void ResetCurCharacter() { delete current_character; current_character = nullptr; }

	// -- 현재 캐릭터 관련 -- //
	void SetCurCharacter(Character* _player);
	Character* GetCurCharacter() { return current_character; }
	bool DeleteCharacter(int _index);
	bool CurCharacter_HP_Down(int _damage);
	void SetCurCharacter_Live() { is_Characterlive = true; }
	void SetCurCharacter_Dead() { is_Characterlive = false; }
	bool GetCurCharacter_IsLive() {	return is_Characterlive; }

	// -- 슬롯관련(슬롯번호,슬롯정보,슬롯초기화) -- //
	bool SetSlot(SlotData* _slotdata);
	const int GetSlotCount() { return slotcount; }
	void SetSlotCount(int _count) { slotcount = _count; }
	bool isSlotLoaded() { return is_slotload; }
	void SlotLoadComplete() { is_slotload = true; }

	// -- 로그인 관련 -- //
	void setID(char *id);
	const char* getID();
	void setPW(char *pw);
	const char* getPW();
	// 로그인,로그아웃 접근지정자
	void setLogin() { login = true; }
	void setLogout() { login = false; }
	// 로그인했는지
	bool isLogin() { return login ? true : false; }

	// -- 파티정보 관련 -- //
	void SetPartyNum(int _partyroomnum) { partyroomnum = _partyroomnum; }
	int GetPartyNum() { return partyroomnum; }
	void SetPartyLeader(bool _leader) { is_partyleader = _leader; }
	bool isParty() { return partyroomnum == -1 ? false : true; }
	bool isPartyLeader() { return is_partyleader ? true : false; }
	void ResetPartyInfo() { partyroomnum = -1; is_partyleader = false; }

	//  -- 던전 관련 -- //
	void SetEnterDungeon() { is_dungeon = true; }
	void SetLeaveDungeon() { is_dungeon = false; }
	bool isDungeon() { return is_dungeon; }
	// void ResetDungeonInfo() { is_dungeon = false; }
	bool GetiStageReady() { return is_gamereday; }
	void SetStageReady() { is_gamereday = true; }
	void SetNotStageReady() { is_gamereday = false; }

	// -- 채널 관련 -- //
	const int GetChannelNum() { return channelnum; }
	void SetChannelNum(int _channelnum) { channelnum = _channelnum; }
	bool isChannel() { return channelnum == -1 ? false : true; }
	
	// -- 인게임 관련 -- //
	void SetEnterGame() { ingame = true; }
	void SetLeaveGame() { ingame = false; }
	bool isIngame() { return ingame ? true : false; }

	// -- 통신관련 -- //
	void SetCallback(bool _callback) { is_callback = _callback; }
	bool GetCallback() { return is_callback; }

	void InitState();
};

#endif
