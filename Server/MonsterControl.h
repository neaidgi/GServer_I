#ifndef MONSTER_CONTROL_H
#define MONSTER_CONTROL_H

#include "GameDataManager.h"
#include "User.h"
#include "CriticalSection.h"
#include "RandomNumberManager.h"
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
	MonsterTime* GetMonsterTime() { return m_monster_time; }

	// ���� Ÿ�̸� ����
	void CreateMonsterTimer();
	// ���� Ÿ�̸� �ð� �ʱ�ȭ
	void ResetMonsterTime();
	// ���� �ð��� ��������
	bool Is_End_MonsterTime();
	// �̸��ʹ� ���������ΰ�
	bool Is_BossMonster();
};

// ���� ��ü�� ��Ʈ���� Ŭ����
class MonsterControl : public CMultiThreadSync<MonsterControl>
{
private:
	// -- �������� ���� -- //
	// ���� ���� ���� Ÿ�̸�
	MonsterTime m_boss_monster_timer;
	// ���� ���Ͱ� ���� ���ΰ�
	bool m_boss_on_the_attack;
	// ���� ���� ���� ��ȣ
	int m_boss_attack_num;
	// ���� ���� ���� �ð�
	float m_boss_attack_max_time;

	// -- ���� ���� �ڷᱸ�� -- //
	// ���� ����
	std::list<MonsterInfo*> m_monsterinfo_list;
	std::list<MonsterInfo*>::iterator m_monsterinfo_save;

	// ���� ���� ����
	int monster_types_num;
	std::vector<int> m_monstercode_vector;
	std::vector<int>::iterator m_monstercode_save;

	/* -- ���� ���� ���� �Լ� -- */
	// ���� ���� Ÿ�̸� ����
	void StartBossMonsterTimer();
	// ���� ���� Ÿ�̸� ����
	bool Is_End_BossMonsterTimer();
public:
	MonsterControl();
	~MonsterControl();

	void StartSearchMonsterinfo();
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	
	// ���� ������ ����ΰ�
	int GetMonsterTypes() {	return m_monstercode_vector.size(); }
	
	// ���� �ڵ� �߰�
	void AddMonsterCode_vector(int _code);
	// ���� �ڵ� ��ȯ
	int GetMonsterCode_vector(int _count);

	// ���� ���� ����
	Monster* MonsterSelect(int _monster_code);

	// -- ���� Ÿ�̸� -- //
	// ���� Ÿ�̸� �ð��ʱ�ȭ
	void InitMonsterTime(int _code, int _num);
	// ���� �ð��� ��������
	bool Is_End_MonsterTime(int _code, int _num);

	/* -- ���� ���� ����,����,�˻�,���� -- */
	// ���� ���� �˻�
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// ���� ����(�����ڵ�,���͹�ȣ) - ���Ӱ� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ) - �⺻�������� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos);
	// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,���͹���) - �⺻�������� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _dir);
	// ���� ���� ����
	bool RemoveMonsterInfo(int _monster_code, int _monster_num);
	// ���� ����Ʈ �ʱ�ȭ(�������� Ŭ���� or ���ο� �����϶� ����Ʈ�� �ʱ�ȭ�ϰ� ���ο� ���� �ֱ��� �۾�)
	void ResetMonsterInfo();
	// ����Ʈ ����ִ���(��������� true �ƴϸ� false)
	bool GetMonsterList_Empty();
	// ���� �������
	int GetMonsterNum(int _code);
	// ���� ü�� ����
	bool Monster_HP_Down(int _monster_code, int _monster_num, int _damage);

	/* -- �������� ���� -- */
	// �������� ���� ����
	void Stage_SetMonster(int _code[],int _num[]);

	/* -- �������� ���� -- */
	// �������� ���� ���� ����
	void Stage_SetBossMonster(int _code, int _num);
	// �̸��ʹ� ���������ΰ�
	bool Is_BossMonster(int _code);
	// ���� ���� ���� ���������� ���� ���� ����(���� ��ȣ,���� �ð�)
	void SetBossMonsterAttackInfo(int _attacknum, int _attacktime);
	// ���� ���� ���� ��ȣ ��������
	int GetBossMonsterAttackNum() {	return m_boss_attack_num; }
	// ���� ���Ͱ� ���� �����Ѱ�
	bool Is_BossMonster_Attackable() { return Is_End_BossMonsterTimer(); }
	// ���� ���ʹ� ���� ������ �Ұ��ΰ�(�����ڵ�,����,[�ƿ�ǲ]���ݹ�ȣ)
	bool Is_BossMonster_What_Attack(MonsterInfo* _monster, float _length, int& _attackcode);


	// �������� ���� �ʱ�ȭ, �����ڵ� getset,����������, ���� �ð� getset
};

#endif