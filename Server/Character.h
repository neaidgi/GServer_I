#ifndef CHARACTER_H
#define CHARACTER_H

#include "Global.h"

// DB���� ������ ���� ������ ĳ���� ��ų
struct AttackInfo
{
	// ��ų�ڵ�
	int attack_code;
	// ��ų����
	float attack_ratio;
	// ��ų����
	int attack_angle;
	// ��ų����
	float attack_range;

	AttackInfo()
	{
		attack_code = 0;
		attack_ratio = 0;
		attack_angle = 0;
		attack_range = 0;
	}

	AttackInfo(int _code, float _ratio, float _range, int _angle)
	{
		attack_code = _code;
		attack_ratio = _ratio;
		attack_angle = _angle;
		attack_range = _range;
	}

	// = ����
	AttackInfo operator=(const AttackInfo& _attackinfo)
	{
		return AttackInfo(attack_code = _attackinfo.attack_code, attack_ratio = _attackinfo.attack_ratio, attack_range = _attackinfo.attack_range, attack_angle = _attackinfo.attack_angle);
	}
};


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
	float character_HP;				// ü��
	float character_MP;				// ����
	float character_current_HP;		// ���� ü��
	float character_current_MP;		// ���� ����
	int	character_attackpoint;		// ���ݷ�
	int	character_defensepoint;		// ����
	int	character_speed;			// �̼�
	int	character_growhealth;		// ���� ü��
	int	character_growmana;			// ���� ����
	int	character_growstr;			// ���� ��
	int	character_growdex;			// ���� ��
	int	character_growint;			// ���� ��
	bool character_jumpstate;		// ��������

	AttackInfo basic_attack;		// �⺻����(1��, ����Ŭ��)
	AttackInfo second_attack;		// ???����(2��, ��Ŭ��)

public:
	const char* GetCharacter_Name() const { return character_name; }
	const char* GetCharacter_JobName() const { return character_jobname; }
	int GetCharacter_JobCode()const { return character_jobcode; }
	const char* GetCharacter_Code() const { return character_code; }
	int GetCharacter_Level() const { return character_level; }
	int GetCharacter_STR() const { return character_str; }
	int GetCharacter_DEX() const { return character_dex; }
	int GetCharacter_INT() const { return character_int; }
	float GetCharacter_HP() const { return character_HP; }
	float GetCharacter_MP() const { return character_MP; }
	float GetCharacter_Current_HP() const { return character_current_HP; }
	float GetCharacter_Current_MP() const { return character_current_MP; }
	int GetCharacter_AttackPoint()const { return character_attackpoint; }
	int GetCharacter_DefensePoint()const { return character_defensepoint; }
	int GetCharacter_Speed()const { return character_speed; }
	int GetCharacter_GrowHealth()const { return character_growhealth; }
	int GetCharacter_GrowMana()const { return character_growmana; }
	int GetCharacter_GrowSTR()const { return character_growstr; }
	int GetCharacter_GrowDEX()const { return character_growdex; }
	int GetCharacter_GrowINT()const { return character_growint; }
	bool GetCharacter_JumpState()const { return character_jumpstate; }

	const Vector3 GetPosition() { return Position; }
	const Vector3 GetRotation() { return Rotation; }
	const Vector3 GetScale() const { return Scale; }

	const AttackInfo GetAttackInfo(int _attack_num)
	{
		switch (_attack_num)
		{
		case 1:
			return basic_attack;
		case 2:
			return second_attack;
		default:
			break;
		}
	}
	AttackInfo GetBasicAttack() const {	return basic_attack; }
	AttackInfo GetSecondAttack() const { return second_attack; }

	// ĳ������ ũ�⸦ ��ȯ(z�� ���̶� ����)
	int GetCharacterRange() const
	{
		if (Scale.x > Scale.y)
		{
			return Scale.x;
		}
		return Scale.y;
	}

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
	void SetCharacter_HP(float _character_HP) { character_HP = _character_HP; }
	void SetCharacter_MP(float _character_MP) { character_MP = _character_MP; }
	void SetCharacter_Current_HP(float _character_current_HP) { character_current_HP = _character_current_HP; }
	void SetCharacter_Current_MP(float _character_current_MP) { character_current_MP = _character_current_MP; }
	void SetCharacter_AttackPoint(int _character_attackpoint) { character_attackpoint = _character_attackpoint; }
	void SetCharacter_DefensePoint(int _character_defensepoint) { character_defensepoint = _character_defensepoint; }
	void SetCharacter_Speed(int _character_speed) { character_speed = _character_speed; }
	void SetCharacter_GrowHealth(int _character_growhealth) { character_growhealth = _character_growhealth; }
	void SetCharacter_GrowMana(int _character_growmana) { character_growmana = _character_growmana; }
	void SetCharacter_GrowSTR(int _character_growstr) { character_growstr = _character_growstr; }
	void SetCharacter_GrowDEX(int _character_growdex) { character_growdex = _character_growdex; }
	void SetCharacter_GrowINT(int _character_growint) { character_growint = _character_growint; }
	void SetCharacter_JumpState(int _character_jumpstate) { character_jumpstate = _character_jumpstate; }

	void SetPosition(const Vector3& _position) { Position = _position; }
	void SetRotation(const Vector3& _rotation) { Rotation = _rotation; }
	void SetScale(const Vector3& _scale) { Scale = _scale; }

	void SetBasicAttack(const AttackInfo _basic_attack) { basic_attack = _basic_attack; }
	void SetSecondAttack(const AttackInfo _second_attack) { second_attack = _second_attack; }

	// ĳ���� ���� ü�� ����
	bool Character_HP_Down(int _damage)
	{
		int current_hp = GetCharacter_Current_HP();
		current_hp = current_hp - _damage;

		if (current_hp <= 0)
		{
			SetCharacter_Current_HP(0);
			return false;
		}
		SetCharacter_Current_HP(current_hp);
		return true;
	}

	Character() 
	{
		character_name = nullptr; 
		character_code = nullptr; 
		character_jobname = nullptr; 
		character_jumpstate = false;
	}
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