#ifndef USER_H
#define USER_H

#include "Packet.h"
#include "UserState.h"
#include "LoginState.h"
#include "TenderState.h"
#include "CharacterState.h"

#define SLOTMAXCOUNT 3 

// DB���� ������ ���� ������ ĳ���� ��ü
class Character
{
private:
	Vector3 Position;				// ��ġ
	Vector3 Rotation;				// ����
	Vector3 Scale;					// ũ��
	char* character_name;			// ĳ���� �г���
	int character_code;				// �ڵ�
	int	character_str;				// ��
	int	character_dex;				// ��
	int	character_int;				// ��
	int	character_health;			// ü��
	int	character_mana;				// ����
	int	character_attackpoint;		// ���ݷ�
	int	character_defensepoint;		// ����
	int	character_speed;			// �̼�
	int	character_growhealth;		// ���� ü��
	int	character_growmana;			// ���� ����
	int	character_growstr;			// ���� ��
	int	character_growdex;			// ���� ��
	int	character_growint;			// ���� ��

public:
	const char* GetCharacter_Name() { return character_name; }
	int GetCharacter_Code() { return character_code; }
	int GetCharacter_STR() { return character_str; }
	int GetCharacter_DEX() { return character_dex; }
	int GetCharacter_INT() { return character_int; }
	int GetCharacter_Health() { return character_health; }
	int GetCharacter_Mana() { return character_mana; }
	int GetCharacter_AttackPoint() { return character_attackpoint; }
	int GetCharacter_DefensePoint() { return character_defensepoint; }
	int GetCharacter_Speed() { return character_speed; }
	int GetCharacter_GrowHealth() { return character_growhealth; }
	int GetCharacter_GrowMana() { return character_growmana; }
	int GetCharacter_GrowSTR() { return character_growstr; }
	int GetCharacter_GrowDEX() { return character_growdex; }
	int GetCharacter_GrowINT() { return character_growint; }

	const Vector3 GetPosition() { return Position; }
	const Vector3 GetRotation() { return Rotation; }
	const Vector3 GetScale() { return Scale; }

	void SetCharacter_Name(char* _character_name) 
	{ 
		int len = strlen(_character_name);
		char* name = new char[len+1];
		memset(name, 0, len + 1);
		strcpy_s(name, len+1,_character_name);
		character_name = name; 
	}
	void SetCharacter_Code(int _character_code) { character_code = _character_code; }
	void SetCharacter_STR(int _character_str) { character_str = _character_str; }
	void SetCharacter_DEX(int _character_dex) { character_dex = _character_dex; }
	void SetCharacter_INT(int _character_int) { character_int = _character_int; }
	void SetCharacter_Health(int _character_health) { character_health = _character_health; }
	void SetCharacter_Mana(int _character_mana) { character_mana = _character_mana; }
	void SetCharacter_AttackPoint(int _character_attackpoint) { character_attackpoint = _character_attackpoint; }
	void SetCharacter_DefensePoint(int _character_defensepoint) { character_defensepoint = _character_defensepoint; }
	void SetCharacter_Speed(int _character_speed) { character_speed = _character_speed; }
	void SetCharacter_GrowHealth(int _character_growhealth) { character_growhealth = _character_growhealth; }
	void SetCharacter_GrowMana(int _character_growmana) { character_growmana = _character_growmana; }
	void SetCharacter_GrowSTR(int _character_growstr) { character_growstr = _character_growstr; }
	void SetCharacter_GrowDEX(int _character_growdex) { character_growdex = _character_growdex; }
	void SetCharacter_GrowINT(int _character_growint) { character_growint = _character_growint; }

	void SetPosition(const Vector3& _position) { Position = _position; }
	void SetRotation(const Vector3& _rotation) { Rotation = _rotation; }
	void SetScale(const Vector3& _scale) { Scale = _scale; }

	~Character() { delete[]character_name; }
};

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
	UserState* state;

	LoginState* loginstate;
	TenderState* tenderstate;
	CharacterState* characterstate;

	SlotData* characterslot[SLOTMAXCOUNT];
	Character* current_character;
	int slotcount;
	bool is_slotload;
public:
	User(SOCKET _sock, SOCKADDR_IN _addr);
	~User();
	UserState* getState();
	LoginState* getLoginState();
	TenderState* getTenderState();
	CharacterState* getCharacterState();
	void SetCurCharacter(Character* _player);
	Character* GetCurCharacter() { return current_character; }
	bool SetSlot(SlotData* _slotdata);
	const SlotData* GetSlot(int _index) { return characterslot[_index - 1]; }		// ���Ե����� ����������
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
	bool isLogin() { return login ? true : false; }

	void InitState();
};

#endif
