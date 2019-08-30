#ifndef MONSTER_H
#define MONSTER_H

#include "Global.h"

using namespace std;

// DB���� ������ ���� ������ ���� ��������
struct MonsterAttackInfo
{
	// �����ڵ�
	int attack_code;
	// ���ݹ���
	int attack_ratio;
	// ���ݰ���
	int attack_angle;
	// ���ݱ���
	int attack_range;
	// ����Ÿ��
	int attack_type;

	MonsterAttackInfo()
	{
		attack_code = 0;
		attack_ratio = 0;
		attack_angle = 0;
		attack_range = 0;
	}

	MonsterAttackInfo(int _code, float _ratio, float _range, int _angle, int _type)
	{
		attack_code = _code;
		attack_ratio = _ratio;
		attack_angle = _angle;
		attack_range = _range;
		attack_type = _type;
	}

	// = ����
	MonsterAttackInfo operator=(const MonsterAttackInfo& _attackinfo)
	{
		return MonsterAttackInfo(attack_code = _attackinfo.attack_code, attack_ratio = _attackinfo.attack_ratio, attack_range = _attackinfo.attack_range, attack_angle = _attackinfo.attack_angle, attack_type = _attackinfo.attack_type);
	}
};

// DB���� ������ ���� ������ ���� ��ü
class Monster
{
private:
	Vector3 m_position;				// ��ġ
	Vector3 m_rotation;				// ����
	Vector3 m_scale;				// ũ��
	char* m_monster_name;			// ���� �̸�
	int m_monster_code;				// �����ڵ�
	int m_monster_num;				// ���� ��ȣ
	float m_monster_HP;				// ü��
	float m_monster_MP;				// ����
	float m_monster_current_hp;		// ���� ü��
	float m_monster_current_mp;		// ���� ����
	int	m_monster_attackpoint;		// ���ݷ�
	int	m_monster_defensepoint;		// ����
	int	m_monster_speed;			// �̼�
	bool m_monster_is_boss;					// ���������ΰ�

	// ���� ����
	vector<MonsterAttackInfo> m_monster_attack_vector;

	MonsterAttackInfo first_attack;		// 1������
	MonsterAttackInfo second_attack;	// 2������

	// ���� Ÿ�� �߰�����(���Ÿ��� �����̳�,�Ϲݸ��̳� �������̳�)
public:

	const char* GetMonster_Name() const { return m_monster_name; }
	int GetMonster_Code() const { return m_monster_code; }
	int GetMonster_Num() const { return m_monster_num; }
	float GetMonster_HP() const { return m_monster_HP; }
	float GetMonster_MP() const { return m_monster_MP; }
	float GetMonster_Current_HP() const { return m_monster_current_hp; }
	float GetMonster_Current_MP() const { return m_monster_current_mp; }
	int GetMonster_AttackPoint() const { return m_monster_attackpoint; }
	int GetMonster_DefensePoint() const { return m_monster_defensepoint; }
	int GetMonster_Speed() const { return m_monster_speed; }
	bool GetMonster_Is_Boss() const { return m_monster_is_boss; }

	const Vector3 GetPosition() const { return m_position; }
	const Vector3 GetRotation() const { return m_rotation; }
	const Vector3 GetScale() const { return m_scale; }
	
	const vector<MonsterAttackInfo> GetAttackInfo() const
	{
		return m_monster_attack_vector;
	}

	const MonsterAttackInfo GetAttackInfo(int _attack_code)
	{
		for (int i = 0; i < m_monster_attack_vector.size(); i++)
		{
			MonsterAttackInfo info = m_monster_attack_vector.at(i);
			if (info.attack_code == _attack_code)
			{
				return info;
			}
		}
	}

	int GetAttackInfoSize() const { return m_monster_attack_vector.size(); }

	MonsterAttackInfo GetFirstAttack() const { return first_attack; }
	MonsterAttackInfo GetSecondAttack() const { return second_attack; }

	// ������ ũ�⸦ ��ȯ(z�� ���̶� ����)
	int GetMonsterRange() const
	{
		if (m_scale.x > m_scale.y)
		{
			return m_scale.x;
		}
		return m_scale.y;
	}

	void SetMonster_Name(const char* _monster_name)
	{
		int len = strlen(_monster_name);
		char* name = new char[len + 1];
		memset(name, 0, len + 1);
		strcpy_s(name, len + 1, _monster_name);
		m_monster_name = name;
	}
	void SetMonster_Code(int _monster_code) { m_monster_code = _monster_code; }
	void SetMonster_Num(int _monster_num) { m_monster_num = _monster_num; }
	void SetMonster_HP(float _monster_health) { m_monster_HP = _monster_health; }
	void SetMonster_MP(float _monster_mana) { m_monster_MP = _monster_mana; }
	void SetMonster_Current_HP(float _monster_current_hp) { m_monster_current_hp = _monster_current_hp; }
	void SetMonster_Current_MP(float _monster_current_mp) { m_monster_current_mp = _monster_current_mp; }
	void SetMonster_AttackPoint(int _monster_attackpoint) { m_monster_attackpoint = _monster_attackpoint; }
	void SetMonster_DefensePoint(int _monster_defensepoint) { m_monster_defensepoint = _monster_defensepoint; }
	void SetMonster_Speed(int _monster_speed) { m_monster_speed = _monster_speed; }
	void SetMonster_Is_Boss(bool _monster_is_boss) { m_monster_is_boss = _monster_is_boss; }

	void SetPosition(const Vector3& _position) { m_position = _position; }
	void SetRotation(const Vector3& _rotation) { m_rotation = _rotation; }
	void SetScale(const Vector3& _scale) { m_scale = _scale; }

	void AddAttackInfo(const MonsterAttackInfo _attack_info) 
	{
		m_monster_attack_vector.push_back(_attack_info);
	}

	void SetFirstAttack(const MonsterAttackInfo _first_attack) { first_attack = _first_attack; }
	void SetSecondAttack(const MonsterAttackInfo _second_attack) { second_attack = _second_attack; }

	Monster()
	{
		m_monster_name = nullptr;
		m_monster_code = 0;
		m_monster_num = 0;
		m_monster_HP = 0;
		m_monster_MP = 0;
		m_monster_current_hp = 0;
		m_monster_current_mp = 0;
		m_monster_attackpoint = 0;
		m_monster_defensepoint = 0;
		m_monster_speed = 0;
		m_monster_is_boss = false;
	}
	~Monster()
	{
		if (m_monster_name != nullptr)
			delete[] m_monster_name;

		m_monster_attack_vector.clear();
	}

	Monster & Monster::operator=(const Monster & rhs)
	{
		m_position = rhs.m_position;
		m_rotation = rhs.m_rotation;
		m_monster_name = rhs.m_monster_name;
		m_monster_code = rhs.m_monster_code;
		m_monster_num = rhs.m_monster_num;
		m_monster_HP = rhs.m_monster_HP;
		m_monster_MP = rhs.m_monster_MP;
		m_monster_current_hp = rhs.m_monster_current_hp;
		m_monster_current_mp = rhs.m_monster_current_mp;
		m_monster_attackpoint = rhs.m_monster_attackpoint;
		m_monster_defensepoint = rhs.m_monster_defensepoint;
		m_monster_speed = rhs.m_monster_speed;
		m_monster_is_boss = rhs.m_monster_is_boss;

		first_attack = rhs.first_attack;
		second_attack = rhs.second_attack;
		return *this;
	}


};

#endif