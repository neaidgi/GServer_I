#ifndef CHARACTER_H
#define CHARACTER_H

#include "Global.h"

// DB���� ������ ���� ������ ĳ���� ��ü
class Character
{
private:
	Vector3 Position;				// ��ġ
	Vector3 Rotation;				// ����
	Vector3 Scale;					// ũ��
	char* character_name;			// ĳ���� �г���
	char* character_jobname;		// ĳ���� �����̸�
	int character_jobcode;			// �����ڵ�
	char* character_code;			// �����ڵ�
	int character_level;			// ����
	int	character_str;				// ��
	int	character_dex;				// ��
	int	character_int;				// ��
	float	character_health;			// ü��
	float	character_mana;				// ����
	int	character_attackpoint;		// ���ݷ�
	int	character_defensepoint;		// ����
	int	character_speed;			// �̼�
	int	character_growhealth;		// ���� ü��
	int	character_growmana;			// ���� ����
	int	character_growstr;			// ���� ��
	int	character_growdex;			// ���� ��
	int	character_growint;			// ���� ��

public:
	const char* GetCharacter_Name() const { return character_name; }
	const char* GetCharacter_JobName() const { return character_jobname; }
	int GetCharacter_JobCode()const { return character_jobcode; }
	const char* GetCharacter_Code() const { return character_code; }
	int GetCharacter_Level() const { return character_level; }
	int GetCharacter_STR() const { return character_str; }
	int GetCharacter_DEX() const { return character_dex; }
	int GetCharacter_INT() const { return character_int; }
	float GetCharacter_Health() const { return character_health; }
	float GetCharacter_Mana() const { return character_mana; }
	int GetCharacter_AttackPoint()const { return character_attackpoint; }
	int GetCharacter_DefensePoint()const { return character_defensepoint; }
	int GetCharacter_Speed()const { return character_speed; }
	int GetCharacter_GrowHealth()const { return character_growhealth; }
	int GetCharacter_GrowMana()const { return character_growmana; }
	int GetCharacter_GrowSTR()const { return character_growstr; }
	int GetCharacter_GrowDEX()const { return character_growdex; }
	int GetCharacter_GrowINT()const { return character_growint; }

	const Vector3 GetPosition() { return Position; }
	const Vector3 GetRotation() { return Rotation; }
	const Vector3 GetScale() { return Scale; }

	void SetCharacter_Name(const char* _character_name)
	{
		int len = strlen(_character_name);
		char* name = new char[len + 1];
		memset(name, 0, len + 1);
		strcpy_s(name, len + 1, _character_name);
		character_name = name;
	}
	void SetCharacter_JobName(const char* _character_jobname)
	{
		int len = strlen(_character_jobname);
		char* jname = new char[len + 1];
		memset(jname, 0, len + 1);
		strcpy_s(jname, len + 1, _character_jobname);
		character_jobname = jname;
	}
	void SetCharacter_JobCode(int _character_code) { character_jobcode = _character_code; }
	void SetCharacter_Code(char* _character_code) 
	{
		int len = strlen(_character_code);
		char* code = new char[len + 1];
		memset(code, 0, len + 1);
		strcpy_s(code, len + 1, _character_code);
		character_code = code;
	}
	void SetCharacter_Level(int _character_level) { character_level = _character_level; }
	void SetCharacter_STR(int _character_str) { character_str = _character_str; }
	void SetCharacter_DEX(int _character_dex) { character_dex = _character_dex; }
	void SetCharacter_INT(int _character_int) { character_int = _character_int; }
	void SetCharacter_Health(float _character_health) { character_health = _character_health; }
	void SetCharacter_Mana(float _character_mana) { character_mana = _character_mana; }
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
	Character() { character_name = nullptr; character_code = nullptr; character_jobname = nullptr; }
	~Character() 
	{ 
		if(character_name != nullptr)
			delete[] character_name; 
		if (character_jobname != nullptr)
			delete[] character_jobname;
		if (character_code != nullptr)
			delete[] character_code; 
	}

	Character& operator=(const Character &rhs);
};

#endif