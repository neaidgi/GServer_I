#ifndef _PARTYSYSTEM_H
#define _PARTYSYSTEM_H

#include "UUIDManager.h"
#include "PartyRoom.h"
#include <list>

#define PARTYMAXUSER 4

// 파티시스템
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
	
	// 파티원들이 스테이지에 입장할 준비가 되었는가
	bool Party_Is_Members_Ready(User* _user);

	// 해당 유저 파티 인원수
	int GetPartyRoomUserNum(int _partyroomnum);
	// 파티방 검색(파티번호)
	PartyRoom* GetPartyRoomSearch(int _partyroomnum);
	
	/* send 담당 */
	// 파티원들에게 send
	void Party_Send_ToMembers(User* _user, UINT64 _p, char * _data, int & _datasize);
	// 특정 파티원에게 send
	void Party_Send_ToOther(User * _user, UINT64 _p, char * _data, int & _datasize);
};


#endif