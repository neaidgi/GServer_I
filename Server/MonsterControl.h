#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "GameDataManager.h"
#include "User.h"
#include "CriticalSectionManager.h"
#include	"MonsterTime.h"

// Ŭ��� ���� �޴� ���� ����(���°��������,���Ͱ�ü,Ȱ��ȭ�Ǿ��ִ����ƴ���)
struct MonsterInfo
{
private:	
	// ���� Ÿ�̸�
	MonsterTime* m_monster_time;
	Monster* m_monster;
	int m_monster_info_num;
	bool m_monster_activate;
public:
	MonsterInfo()
	{
		m_monster_time = nullptr;
		m_monster = nullptr;
		m_monster_info_num = -1;
		m_monster_activate = false;
	}
	~MonsterInfo()
	{
		if (m_monster_time != nullptr)
		{
			delete m_monster_time;
			m_monster_time = nullptr;
		}

		if (m_monster != nullptr)
		{
			delete m_monster;
			m_monster = nullptr;
		}
	}
	// ���� ����������
	void SetMonster(Monster* _monster) { m_monster = _monster; }
	Monster* GetMonster() { return m_monster; }

	// ���� ��ȣ ����������
	void SetMonsterNum(int _num) { m_monster_info_num = _num; }
	int GetMonsterNum() { return m_monster_info_num; }

	// ���� Ÿ�̸� ����������
	void SetMonsterTime(MonsterTime* _monstertime) { m_monster_time = _monstertime; }
	MonsterTime* GetMonsterTime() { return m_monster_time; }

	// ���� Ÿ�̸� �ð��ʱ�ȭ
	void InitMonsterTime();
	// ���� �ð��� ��������
	bool Is_End_MonsterTime();

};

// ���� ��ü�� ��Ʈ���� Ŭ����
class MonsterControl
{
private:
	// ���� ����
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;

	// ���� ���� ����
	int number_monster_types;
	std::vector<MONSTER_ORIGINCODE> m_monstercode_vector;
	std::vector<MONSTER_ORIGINCODE>::iterator m_monstercode_save;
public:

	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	
	int GetMonsterTypes() {	return m_monstercode_vector.size(); }
	
	// ���� �ڵ� �߰�
	void AddMonsterCode_vector(MONSTER_ORIGINCODE _code);
	// ���� �ڵ� ��ȯ
	MONSTER_ORIGINCODE GetMonsterCode_vector(int _count);

	// ���� ���� ����
	Monster* MonsterSelect(int _monster_code);
	// ���� Ÿ�̸� �߰�
	MonsterTime* AddMonsterTimer();

	// ���� Ÿ�̸� �ð��ʱ�ȭ
	void InitMonsterTime(int _code, int _num);
	// ���� �ð��� ��������
	bool Is_End_MonsterTime(int _code, int _num);

	// ���� ���� �˻�
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// ���� ����(�����ڵ�,���͹�ȣ) - ���Ӱ� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,����ȸ����) - �⺻�������� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos);
	// ���� ���� ����
	bool RemoveMonsterInfo(int _monster_code, int _monster_num);
	// ���� ����Ʈ �ʱ�ȭ(�������� Ŭ���� or ���ο� �����϶� ����Ʈ�� �ʱ�ȭ�ϰ� ���ο� ���� �ֱ��� �۾�)
	void ResetMonsterInfo();
	// ����Ʈ ����ִ���(��������� true �ƴϸ� false)
	bool GetMonsterList_Empty();
	// ù��° �������� �Ϲݸ� ����
	void SetFirstStage_NormalMonster();
};

#endif