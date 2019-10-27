#ifndef PARTYROOM_H
#define PARTYROOM_H

#define PARTYMAXUSER 4

#include "MonsterControl.h"
#include "MonsterTime.h"
#include "User.h"
#include <list>

// ��Ƽ��
class PartyRoom
{
private:
	// ��Ƽ ��ȣ
	int partyroom_num;
	// ��Ƽ ���� ����Ʈ
	std::list<User*> m_partyuser;
	std::list<User*>::iterator save;
	// ���� ��Ʈ�ѷ�
	MonsterControl* m_monster_control;
	// �������� ����
	StageInfo* m_stage_info;

public:
	PartyRoom();
	PartyRoom(int _partyroomnum);
	~PartyRoom();

	// �ʱ�ȭ
	void InitPartyRoom();
	// ������
	void DestroyPartyRoom();

	/* -- ��Ƽ ���� ���� -- */
	// �˻� �ʱ�ȭ
	void StartSearchPartyRoom();
	// ��Ƽ��(����) ����
	bool SearchPartyRoom(User*& _user);
	// ��Ƽ�� ��ȣ
	int GetPartyRoomNum();
	// ���� ��Ƽ�� �ο���
	int GetPartRoomUserNum();
	// ���� �߰�
	void PartyRoomAddUser(User* _user);
	// ���� �߰�(�ڵ�)
	// -- �ȸ���
	// ���� ����
	bool PartyRoomRemoveUser(User* _user);
	// ���� ����(�ڵ�)
	bool PartyRoomRemoveUser(char* _code);
	// ��Ƽ�� �Ӹ�
	bool AppointPartyLeader(User* _user);
	// ��Ƽ�� �Ӹ�
	bool AppointPartyLeader(char* _code);
	// ��Ƽ�� ����
	bool DismissPartyLeader(User* _user);
	// ��Ƽ�� ����
	bool DismissPartyLeader(char* _code);
	// ��Ƽ���� �����ΰ�
	User* WhopartyLeader();

	/* -- ��Ƽ ���� ���� -- */
	// ���� ����
	void DungeonEnter_PartyRoom();
	// ���� ����
	void DungeonLeave_PartyRoom();

	/* -- �������� ���� -- */
	// �������� ����
	void SetDungeonStage(int _stage_num) { m_stage_info->SetStage_Num(_stage_num); }
	// ���� ���° ������������
	int GetDungeonStage() { return m_stage_info->GetStage_Num(); }
	// �������� ���
	void RiseStage();
	// ���������� �´� ���� ���� ����
	void SetDungeonMonsterinfo();
	// ���� ���� ������������
	bool GetIsBossStage() { return m_stage_info->GetStage_Is_Boss_Stage(); }

	/* -- ���� Ÿ�̸� ���� -- */
	// ���� �ð� �ʱ�ȭ
	void Start_MonsterTimer(int _code, int _num);
	// ���� �ð� �������� Ȯ��
	bool End_MonsterTimer(int _code, int _num);

	/* -- ���� ���� ���� -- */
	// ���� ������ �������
	int GetMonsterTypes() { return m_monster_control->GetMonsterTypes(); }
	// ���� �˻� �ʱ�ȭ
	void StartSearchMonsterinfo();
	// ���� �˻�
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	// ���� ���� ��û
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// ���� ����(�����ڵ�,���͹�ȣ) - ���Ӱ� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ) - �⺻�������� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos);
	// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,���͹���) - �⺻�������� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos, const Vector3 _dir);
	// ���� ���� ����
	bool RemoveMonsterInfo(int _monster_code, int _monster_num);
	// ���� ü�� ����. ������ false. ��������� true
	bool Monster_HP_Down(int _monster_code, int _monster_num, int _damage);
	// ���� �ڵ� ��ȯ
	int GetMonsterCode(int _code);
	// Ư�� ��Ŀ ��� ����
	int GetMonsterNum(int _code);

	/* -- ���� ���� ���� -- */
	// �̸��Ͱ� ������������
	bool Is_BossMonster(int _code);
	// ���� ���� ���� ���������� ���� ���� ����(���� ��ȣ,���� �ð�)
	void SetBossMonsterAttackInfo(int _attacknum, int _attacktime);
	// ���� ���� ���� ��ȣ ��������
	int GetBossMonsterAttackNum() { return m_monster_control->GetBossMonsterAttackNum(); }
	// ���� ���Ͱ� ���� �����Ѱ�
	bool Is_BossMonster_Attackable() { return m_monster_control->Is_BossMonster_Attackable(); }
	// ���� ���ʹ� ���� ������ �Ұ��ΰ�(�����ڵ�,����,[�ƿ�ǲ]���ݹ�ȣ)
	bool Is_BossMonster_What_Attack(MonsterInfo* _monster, float _length, int& _attackcode);
};

#endif
