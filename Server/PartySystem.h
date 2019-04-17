#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "User.h"
#include "UUIDManager.h"
#include "MonsterControl.h"
#include <list>

#define PARTYMAXUSER 4

// 파티방
class PartyRoom
{
private:
	// 파티 번호
	int partyroom_num;			
	// 던전 스테이지 번호
	DUNGEON_STAGE m_dungeon_stage_num;
	// 파티 유저 리스트
	std::list<User*> m_partyuser;	
	std::list<User*>::iterator save;
	
	// 몬스터 컨트롤러
	MonsterControl* m_monster_control;

public:
	PartyRoom(int _partyroomnum);
	~PartyRoom();

	// 초기화
	void InitPartyRoom();
	// 마무리
	void DestroyPartyRoom();

	// 검색 초기화
	void StartSearchPartyRoom();
	// 파티원(유저) 전달
	bool SearchPartyRoom(User*& _user);
	// 파티방 번호
	int GetPartyRoomNum();
	// 현재 파티원 인원수
	int GetPartRoomUserNum();
	// 유저 추가
	void PartyRoomAddUser(User* _user);
	// 유저 추가(코드)

	// 유저 삭제
	bool PartyRoomRemoveUser(User* _user);
	// 유저 삭제(코드)
	bool PartyRoomRemoveUser(char* _code);
	// 파티장 임명
	bool AppointPartyLeader(User* _user);
	// 파티장 임명
	bool AppointPartyLeader(char* _code);
	// 파티장 해임
	bool DismissPartyLeader(User* _user);
	// 파티장 해임
	bool DismissPartyLeader(char* _code);
	// 파티장이 누구인가
	User* WhopartyLeader();
	// 던전 입장
	void DungeonEnter_PartyRoom();
	// 던전 퇴장
	void DungeonLeave_PartyRoom();


	// 스테이지 변경
	void SetDungeonStage(DUNGEON_STAGE _stage_num) { m_dungeon_stage_num = _stage_num; }
	// 지금 몇번째 스테이지인지
	DUNGEON_STAGE GetDungeonStage() { return m_dungeon_stage_num; }
	// 스테이지 상승
	void RiseStage();
	// 스테이지에 맞는 몬스터 정보 셋팅
	void SetDungeonMonsterinfo();

};

// 파티시스템
class PartySystem
{
private:
	std::list<PartyRoom*> m_partyroom_list;
	std::list<PartyRoom*>::iterator save;
public:
	PartySystem();
	~PartySystem();

	void InitializePartySystem();
	// 검색 초기화
	void StartSearchPartyRoom();

	// 유저 추가
	bool Partyroom_add_User(User* _user, int _partyroomnum);
	// 파티생성
	bool Party_Create(User* _user);
	// 파티참여

	// 파티 초대 가능한지
	bool Party_Invite(User* _user, User* _targetuser);
	// 파티 삭제
	bool Party_Remove(User* _user);
	// 파티탈퇴
	bool Party_User_Leave(User* _user);
	// 파티 강퇴
	bool Party_Kick_User(User* _user,char* _code);
	// 파티원들에게 정보보내기(코드, 체력, 마나)

	// 파티장 위임
	bool Party_Leader_Delegate(User* _user, char* _code);

	// 해당 유저 파티 인원수
	int GetPartyRoomUserNum(int _partyroomnum);
	// 파티방 검색(파티번호)
	PartyRoom* GetPartyRoomSearch(int _partyroomnum);
};


#endif