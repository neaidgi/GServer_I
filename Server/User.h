#ifndef USER_H
#define USER_H

#include "Packet.h"
#include "UserState.h"
#include "LoginState.h"
#include "TenderState.h"
#include "CharacterState.h"

#define SLOTMAXCOUNT 3 

class Character
{
private:
	char* character_name;		// 캐릭터 이름
	int character_code;				// 코드
	int	character_str;				// 힘
	int	character_dex;				// 민
	int	character_int;				// 지
	int	character_health;			// 체력
	int	character_attackpoint;		// 공격력
	int	character_defensepoint;		// 방어력
	int	character_speed;			// 이속
	int	character_growhealth;		// 성장 체력
	int	character_growstr;		// 성장 힘
	int	character_growdex;		// 성장 민
	int	character_growint;		// 성장 지

public:
	const char* GetCharacter_Name() { return character_name; }
	int GetCharacter_Code() { return character_code; }
	int GetCharacter_STR() { return character_str; }
	int GetCharacter_DEX() { return character_dex; }
	int GetCharacter_INT() { return character_int; }
	int GetCharacter_Health() { return character_health; }
	int GetCharacter_AttackPoint() { return character_attackpoint; }
	int GetCharacter_DefensePoint() { return character_defensepoint; }
	int GetCharacter_Speed() { return character_speed; }
	int GetCharacter_GrowHealth() { return character_growhealth; }
	int GetCharacter_GrowSTR() { return character_growstr; }
	int GetCharacter_GrowDEX() { return character_growdex; }
	int GetCharacter_GrowINT() { return character_growint; }


};


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
	CharacterState* characterstate;

	Character* characterslot[SLOTMAXCOUNT];
public:
	User(SOCKET _sock, SOCKADDR_IN _addr);
	~User();
	UserState* getState();
	LoginState* getLoginState();
	TenderState* getTenderState();
	CharacterState* getCharacterState();
	void SetCharacter(int _character_code);
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
