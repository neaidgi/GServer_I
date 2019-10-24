#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "User.h"
#include "UUIDManager.h"
#include "MonsterControl.h"
#include "MonsterTime.h"
#include <list>

#define PARTYMAXUSER 4

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

// ��Ƽ�ý���
class PartyManager
{
private:
	static PartyManager* Instance;

	std::list<PartyRoom*> m_partyroom_list;
	std::list<PartyRoom*>::iterator save;
public:
	PartyManager();
	~PartyManager();

	static void CreateInstance();
	static PartyManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void InitializePartySystem();
	// �˻� �ʱ�ȭ
	void StartSearchPartyRoom();

	// ���� �߰�
	bool Partyroom_add_User(User* _user, int _partyroomnum);
	// ��Ƽ����
	bool Party_Create(User* _user);
	// ��Ƽ����

	// ��Ƽ �ʴ� ��������
	bool Party_Invite(User* _user, User* _targetuser);
	// ��Ƽ ����
	bool Party_Remove(User* _user);
	// ��ƼŻ��
	bool Party_User_Leave(User* _user);
	// ��Ƽ ����
	bool Party_Kick_User(User* _user,char* _code);
	// ��Ƽ���鿡�� ����������(�ڵ�, ü��, ����)

	// ��Ƽ�� ����
	bool Party_Leader_Delegate(User* _user, char* _code);
	
	// ��Ƽ������ ���������� ������ �غ� �Ǿ��°�
	bool Party_Is_Members_Ready(User* _user);

	// �ش� ���� ��Ƽ �ο���
	int GetPartyRoomUserNum(int _partyroomnum);
	// ��Ƽ�� �˻�(��Ƽ��ȣ)
	PartyRoom* GetPartyRoomSearch(int _partyroomnum);
};


#endif