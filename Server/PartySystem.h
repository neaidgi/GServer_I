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
	// ���� ����
	void DungeonEnter_PartyRoom();
	// ���� ����
	void DungeonLeave_PartyRoom();


	// �������� ����
	void SetDungeonStage(int _stage_num) { m_stage_info->SetStage_Num(_stage_num); }
	// ���� ���° ������������
	int GetDungeonStage() { return m_stage_info->GetStage_Num(); }
	// ���� ������ �������
	int GetMonsterTypes() { return m_monster_control->GetMonsterTypes(); }
	// ���� ���� ������������
	bool GetIsBossStage() { return m_stage_info->GetStage_Is_Boss_Stage(); }

	// ���� �˻� �ʱ�ȭ
	void StartSearchMonsterinfo();
	// ���� �˻�
	bool SearchMonsterinfo(MonsterInfo*& _monsterinfo);
	// �������� ���
	void RiseStage();
	// ���������� �´� ���� ���� ����
	void SetDungeonMonsterinfo();
	// ���� �ð� �ʱ�ȭ
	void Start_MonsterTimer(int _code, int _num);
	// ���� �ð� �������� Ȯ��
	bool End_MonsterTimer(int _code, int _num);
	// ���� �ڵ� ��ȯ
	int GetMonsterCode(int _code);
	// Ư�� ��Ŀ ��� ����
	int GetMonsterNum(int _code);
	// ���� ���� ��û
	bool GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo*& _monsterinfo);
	// ���� ����(�����ڵ�,���͹�ȣ) - ���Ӱ� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num);
	// ���� ���� ����(�����ڵ�,���͹�ȣ,������ǥ,����ȸ����) - �⺻�������� �����Ҷ�
	void SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos);
	// ���� ���� ����
	bool RemoveMonsterInfo(int _monster_code, int _monster_num);
	// ���� ü�� ����. ������ false. ��������� true
	bool Monster_HP_Down(int _monster_code, int _monster_num, int _damage);
};

// ��Ƽ�ý���
class PartySystem
{
private:
	std::list<PartyRoom*> m_partyroom_list;
	std::list<PartyRoom*>::iterator save;
public:
	PartySystem();
	~PartySystem();

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

	// �ش� ���� ��Ƽ �ο���
	int GetPartyRoomUserNum(int _partyroomnum);
	// ��Ƽ�� �˻�(��Ƽ��ȣ)
	PartyRoom* GetPartyRoomSearch(int _partyroomnum);
};


#endif