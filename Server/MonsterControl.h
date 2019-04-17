#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "GameDataManager.h"
#include "User.h"
#include "CriticalSectionManager.h"

// Ŭ��� ���� �޴� ���� ����(���°��������,���Ͱ�ü,Ȱ��ȭ�Ǿ��ִ����ƴ���)
struct MonsterInfo
{
private:
	Monster* m_monster;
	int m_monster_info_num;
	bool m_monster_activate;
public:
	MonsterInfo()
	{
		m_monster_info_num = -1;
		m_monster = nullptr;
		m_monster_activate = false;
	}
	~MonsterInfo()
	{
		delete m_monster;
		m_monster = nullptr;
	}
	// ���� ����������
	void SetMonster(Monster* _monster) { m_monster = _monster; }
	Monster* GetMonster() { return m_monster; }

	// ���� ��ȣ ����������
	void SetMonsterNum(int _num) { m_monster_info_num = _num; }
	int GetMonsterNum() { return m_monster_info_num; }

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
public:

	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	
	// ���� ���� ����
	Monster* MonsterSelect(int _monster_code);
	// ���� ���� �˻�
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// ���� ����(�����ڵ�,���͹�ȣ) - ���Ӱ� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,����ȸ����) - �⺻�������� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _rot);
	// ���� ����Ʈ �ʱ�ȭ(�������� Ŭ���� or ���ο� �����϶� ����Ʈ�� �ʱ�ȭ�ϰ� ���ο� ���� �ֱ��� �۾�)
	void ResetMonsterInfo();
	// ����Ʈ ����ִ���(��������� true �ƴϸ� false)
	bool GetMonsterList_Empty();
	// ù��° �������� �Ϲݸ� ����
	void SetFirstStage_NormalMonster();
};

#endif