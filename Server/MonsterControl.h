#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "GameDataManager.h"
#include "User.h"
#include "CriticalSectionManager.h"
#include "MonsterTime.h"

// Ŭ��� ���� �޴� ���� ����(���°��������,���Ͱ�ü,Ȱ��ȭ�Ǿ��ִ����ƴ���)
struct MonsterInfo
{
private:	
	// ���� Ÿ�̸�
	MonsterTime* m_monster_time;
	// ����
	Monster* m_monster;
	// ���� ��ȣ
	int m_monster_info_num;
	// ���Ͱ� �׾����� ��Ҵ���. 
	bool m_monster_activate;
public:
	MonsterInfo();
	~MonsterInfo();
	// ���� ����������
	void SetMonster(Monster* _monster) { m_monster = _monster; }
	Monster* GetMonster() { return m_monster; }

	// ���� ��ȣ ����������
	void SetMonsterNum(int _num) { m_monster_info_num = _num; }
	int GetMonsterNum() { return m_monster_info_num; }

	// ���� ����(�׾����� ��Ҵ���) ����������
	void SetMonsterDie() { m_monster_activate = false; }
	void SetMonsterlive() { m_monster_activate = true; }
	bool GetMonsterActivate() { return m_monster_activate; }

	// ���� Ÿ�̸� ����������
	void SetMonsterTime(MonsterTime* _monstertime) { m_monster_time = _monstertime; }
	MonsterTime* GetMonsterTime() { return m_monster_time; }

	// ���� Ÿ�̸� �ð��ʱ�ȭ
	void InitMonsterTime();
	// ���� �ð��� ��������
	bool Is_End_MonsterTime();

};

// ���� ��ü�� ��Ʈ���� Ŭ����
class MonsterControl : public CMultiThreadSync<MonsterControl>
{
private:
	// ���� ����
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;

	// ���� ���� ����
	int monster_types_num;
	std::vector<int> m_monstercode_vector;
	std::vector<int>::iterator m_monstercode_save;
public:
	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	
	int GetMonsterTypes() {	return m_monstercode_vector.size(); }
	
	// ���� �ڵ� �߰�
	void AddMonsterCode_vector(int _code);
	// ���� �ڵ� ��ȯ
	int GetMonsterCode_vector(int _count);

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
	// �������� ���� ����
	void Stage_SetMonster(int _code[],int _num[]);
	// �������� ���� ���� ����
	void Stage_SetBossMonster(int _code, int _num);
	// ���� �������
	int GetMonsterNum(int _code);
	// ���� ü�� ����
	bool Monster_HP_Down(int _monster_code, int _monster_num, int _damage);
};

#endif