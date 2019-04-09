#ifndef MONSTER_H
#define MONSTER_H

#include "Global.h"

// DB에서 가져온 정보 저장할 몬스터 객체
class Monster
{
private:
	Vector3 m_position;				// 위치
	Vector3 m_rotation;				// 방향
	Vector3 m_scale;				// 크기
	char* m_monster_name;			// 몬스터 이름
	int m_monster_code;			// 고유코드
	int m_monster_num;				// 몬스터 번호
	float m_monster_health;			// 체력
	float m_monster_mana;			// 마나
	int	m_monster_attackpoint;		// 공격력
	int	m_monster_defensepoint;		// 방어력
	int	m_monster_speed;			// 이속
	// 몬스터 타입 추가예정(원거리냐 근접이냐,일반몹이냐 보스몹이냐)
public:

	const char* GetMonster_Name() const { return m_monster_name; }
	int GetMonster_Code() const { return m_monster_code; }
	int GetMonster_Num() { return m_monster_num; }
	float GetMonster_Health() { return m_monster_health; }
	float GetMonster_Mana() { return m_monster_mana; }
	int GetMonster_AttackPoint() { return m_monster_attackpoint; }
	int GetMonster_DefensePoint() { return m_monster_defensepoint; }
	int GetMonster_Speed() { return m_monster_speed; }

	const Vector3 GetPosition() { return m_position; }
	const Vector3 GetRotation() { return m_rotation; }
	const Vector3 GetScale() { return m_scale; }

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
	void SetMonster_Health(float _monster_health) { m_monster_health = _monster_health; }
	void SetMonster_Mana(float _monster_mana) { m_monster_mana = _monster_mana; }
	void SetMonster_AttackPoint(int _monster_attackpoint) { m_monster_attackpoint = _monster_attackpoint; }
	void SetMonster_DefensePoint(int _monster_defensepoint) { m_monster_defensepoint = _monster_defensepoint; }
	void SetMonster_Speed(int _monster_speed) { m_monster_speed = _monster_speed; }

	void SetPosition(const Vector3& _position) { m_position = _position; }
	void SetRotation(const Vector3& _rotation) { m_rotation = _rotation; }
	void SetScale(const Vector3& _scale) { m_scale = _scale; }

	Monster() { m_monster_name = nullptr; }
	~Monster()
	{
		if (m_monster_name != nullptr)
			delete[] m_monster_name;
	}

	Monster & Monster::operator=(const Monster & rhs)
	{
		m_position = rhs.m_position;
		m_rotation = rhs.m_rotation;
		m_monster_name = rhs.m_monster_name;
		m_monster_code = rhs.m_monster_code;
		m_monster_num = rhs.m_monster_num;
		m_monster_health = rhs.m_monster_health;
		m_monster_mana = rhs.m_monster_mana;
		m_monster_attackpoint = rhs.m_monster_attackpoint;
		m_monster_defensepoint = rhs.m_monster_defensepoint;
		m_monster_speed = rhs.m_monster_speed;

		return *this;
	}

};

#endif