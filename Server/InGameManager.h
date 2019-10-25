#ifndef _INGAME_MANAGER_H_
#define _INGAME_MANAGER_H_

#include "Global.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"
#include "UserManager.h"
#include "DBManager.h"
#include "CharacterVerification.h"
#include "CriticalSection.h"
#include "ChannelSystem.h"
#include "GameDataManager.h"
#include "ThreadManager.h"
#include "RandomNumberManager.h"

/*
	[InGameManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

// 동기화를 사용하기 위해 CMultiThreadSync 상속받음
class InGameManager : public CMultiThreadSync<InGameManager>
{
private:
	static InGameManager* Instance;

	InGameManager();
	~InGameManager();

	CharacterVerification* m_verification;
	ChannelSystem*	channelsystem;
	PartyManager* m_partysystem;

	// **Pack 함수** //
	// 다른 유저 위치정보 요청. (유저숫자,코드,직업코드,닉네임,위치,회전)
	void User_Pack_OtherUserPosData(User * _user);
	// 접속한 유저 정보 패킹. (코드,직업코드,닉네임,위치,회전)
	void User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize);
	// 이동 요청 
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);
	//  회전 정보.  (코드,회전값(vector3))
	void User_Pack_Rotation(User*_user, char* _data, int& _datasize);
	// 채널 정보 전송.(채널번호,채널유저수....x6)
	void User_Pack_ChannelInfo(User* _user);
	// 채널 이동 결과 패킹. (성공실패,채널번호)
	bool User_Pack_ChannelChangeResult(User* _user, char* _buf, int _choice, int& _datasize, int& _oldchannelnum);
	// 현재 접속한 채널몇번이지 패킹(채널번호)
	void User_Pack_Current_ChannelInfo(User* _user, char* _buf, int& _datasize);
	// 파티 결과 패킹. (결과)
	void User_Pack_Party_Result(User * _user, bool _result, char* _data, int& _datasize);
	// 파티 결과 및 코드 패킹. (결과,성공이면 코드 실패면 끝)
	void User_Pack_Party_Result_Code(User* _user, bool _result, char* _code, char* _data, int& _datasize);
	// 던전 입장시 스폰 지역 패킹(스폰좌표)
	void User_Pack_Dungeon_SpawnData(User * _user,int _count, char* _data, int& _datasize);

	// 던전 퇴장시 채널정보 패킹. 채널번호

	// 몬스터 이동정보 패킹(몬스터숫자,몬스터코드,몬스터번호,몬스터좌표)
	void User_Pack_Monster_MoveInfo(User* _user, int _code, int _num, char*& _data, int& _datasize, UINT64& _sendprotocol);
	// 몬스터 피격결과 정보 패킹(성공or실패,몬스터코드,몬스터번호,입힌피해량,죽었는지)
	void User_Pack_MonsterAttack_Result(User* _user, bool _result, int _monstercode, int _monsternum, int _damage, bool _isdie);
	// 유저 피격결과 정보패킹(성공실패,데미지,죽으면 false)
	void User_Pack_Under_Attack_Result(User* _user, bool _result, int _damage, bool _state);
	// 몬스터 타겟 정보패킹(몬스터코드,몬스터번호,케릭터코드)
	void User_Pack_Nearset_Character(User* _user, int _monstercode, int _monsternum, Character* _character, char*& _data, int& _datasize, UINT64& _sendprotocol);

	// **UnPack 함수**
	// 채널 이동 요청 언팩 (채널번호)
	void User_UnPack_ChannelChange(User* _user, char* _buf, int& _channelnum);
	// 캐릭터 코드 언팩 (코드)
	void User_Unpack_UserCode(User* _user, char* _buf, char* _code);
	// 초대 요청 언팩 (결과,코드,파티번호)
	void User_Unpack_PartyRoom_Invite_Result(User* _user, char*_buf, bool& _result, char* _code, int& _partyroomnum);
	// 몬스터 이동 정보 언팩(몬스터코드,몬스터번호,좌표)
	void User_Unpack_Monster_Move(User* _user, char* _buf, int& _code, int& _num);
	// 몬스터 이동 정보 언팩(몬스터코드,몬스터번호,좌표,방향)
	void User_Unpack_BossMonster_Move(User* _user, char* _buf, int& _code, int& _num);
	// 유저가 특정 몬스터를 공격했다는 패킷(몬스터 코드,몬스터 번호, 공격번호, 유저의 방향벡터)
	void User_Unpack_User_Successfully_Attack_The_Monster(User* _user, char* _buf, int& _monstercode, int& _monsternum, int& _attacknum, Vector3& _dir);
	// 유저가 무슨공격했는지 언팩(공격정보)
	void User_Unpack_Attack_Info(User* _user, char* _buf, int& _attacknum);
	// 몬스터가 무슨공격했는지 언팩(공격정보)
	void User_Unpack_Monster_Successfully_Attack_The_User(User* _user, char* _buf, int& _monstercode, int& _monsternum, int& _attacknum, Vector3& _dir);
	

	// ** 다른유저에게 줄 정보 Pack ** //
	// 이동이나 회전 정보 패킹 (코드,백터3)
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize);
	// 유저 코드 패킹. (지금 해당 유저의 코드)
	void User_Pack_UserCode(User* _user, char* _data, int& _datasize);
	// 파티 초대 패킹(코드,닉네임,파티번호)
	void User_Pack_Party_InviteToOther(User* _user, char* _data, int& _datasize);
	// 파티에 속해있는 유저정보 패킹(파티번호,유저숫자,코드,직업코드,닉네임,Hp,Mp,방장인지)
	void User_Pack_Party_CharacterInfoToOther(User* _user, char* _data, int& _datasize);
	// 결과패킹 (bool)
	void User_Pack_Party_InviteResultToOther(User* _user, char* _data, int& _datasize, bool _result);
	// 파티 프로토콜 패킹 (프로토콜)
	void User_Pack_Party_Protocol(User* _user, char* _data, int& _datasize);
	// 파티 강퇴 당한 유저 정보 패킹. (특정 유저의 코드)
	void User_Pack_Party_KickInfo(User* _user, char* _data, int& _datasize, char*_code);
	// 파티 리더 바뀐 유저 정보 패킷. (코드, 코드)
	void User_Pack_PartyRoom_Leader_Delegate(User* _user, char* _data, int& _datasize, char* _code);
	// 던전 입장시 스폰위치. (파티원수,유저코드,좌표)
	void User_Pack_Party_Dungeon_SpawnData(User* _user,char* _data,int& _datasize);
	// 던전 스테이지 입장시 스폰위치. (파티원수,유저코드,좌표)
	void User_Pack_Party_Dungeon_Stage_SpawnData(User* _user, UINT64 _p, char* _data, int& _datasize);
	// 던전 스테이지 입장시 몬스터 정보.
	void User_Pack_Dungeon_Monster_SpawnInfo(User* _user, char* _data, int& _datasize);
	// 던전 스테이지 입장시 몬스터 정보.(몬스터코드,몬스터숫자,스폰될좌표)
	void User_Pack_Dungeon_Monster_SpawnInfo(User* _user, char* _data, int& _datasize, int _code, int _num, Vector3 _pos);
	// 공격정보 패킹(유저코드,공격정보)
	void User_Pack_AttackNum_Info(User* _user, char* _data, int& _datasize, int _attacknum);
	// 공격정보 패킹(몬스터코드,몬스터번호,공격번호)
	void User_Pack_Monster_Attack_Info(User* _user, char* _data, int& _datasize,int _code, int _num, int _attackcode);
	// 몬스터 피격결과. 다른유저한테 정보패킹(데미지,죽었다는의미)
	void User_Pack_MonsterAttackToOher_Result(User* _user, char* _data, int& _datasize, int _monstercode, int _monsternum, int _damage, bool _isdie);
	// 유저 피격결과. 다른유저한테 정보패킹(캐릭터코드,데미지,죽었다는의미)
	void User_Pack_User_UnderAttackToOher_Result(User* _user, char* _data, int& _datasize,  int _damage, bool _isdie);


	// ** 유저 캐릭터 관련 ** //
	// 현재 캐릭터 DB에 저장
	void User_CurCharacter_Save(User* _user);
	// 해당 유저가 파티에 속해있는지
	bool User_IsParty(char* _code);
	// 유저 공격시 피격 판정
	bool User_Attack_Result(User* _user, char* _buf);

	// **Send 함수** //
	// 다른 유저들한테 보내는 send함수 통합(enum문으로 타입을 정해서 만들자)
	void User_Send_ToOther(User* _user, UINT64 _p, SEND_TYPE _type, char* _data, int _datasize, int _channelnum, char* _code);
	// 채널에 속해있는 유저들한테 전송
	void User_Send_In_The_Channel(User* _user, UINT64 _p, char* _data, int& _datasize);
	// 특정 채널에 속해있는 유저들한테 전송(채널이동할때 사용)
	void User_Send_In_a_Particular_Channel(User* _user, UINT64 _p, int _channelnum, char* _data, int& _datasize);
	// 다른 유저 인게임에서 떠난 정보 채널에 전송(파티원 제외)
	void User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(User* _user, UINT64 _p, int _channelnum, char* _data, int& _datasize);
	// 특정 유저(code) 파티 초대 전송
	void User_Send_Party_InviteToOther(User* _user, UINT64 _p, char* _data, int& _datasize, char* _code);
	// 파티원에게 전송
	void User_Send_Party_ToOther(User* _user, UINT64 _p, char* _data, int& _datasize);
	// 파티원에게 전송(해당 유저 포함)
	void User_Send_Party(User* _user, UINT64 _p, char* _data, int& _datasize);


	// 다른 유저 인게임에서 떠난 정보 전송
	void User_Send_LeaveInfoToOther(User* _user, UINT64 _p, char* _data, int& _datasize);
	// 던전에 들어갔을때 채널에 속해있는 유저들한테 전송
	void User_Send_Party_Enter_Dungeon(User* _user);
	// 던전에 나갔을때 채널에 속해있는 유저들한테 전송
	void User_Send_Party_Leave_Dungeon(User* _user);


	//  **몬스터 관련** //
	// 몬스터 공격시 피격 판정
	bool User_Under_Attack(User* _user, char* _buf);
	// 스테이지 상승 및 몬스터정보 셋팅
	void User_Dungeon_Stage_Rise(User* _user);
	// 몬스터 시간 초기화
	void User_PartyRoom_Monster_Time_ReSet(User* _user, int _code, int _num);
	// 해당 유저의 파티의 몬스터 시간이 정해둔 시간을 지났는가.
	bool User_PartyRoom_Monster_TimeOver_Check(User* _user, int _code, int _num);
	// 이몬스터는 보스몬스터인가
	bool Is_BossMonster(User* _user, int _code);
	// 몬스터 정보 가져오기
	bool GetMonsterInfo(User* _user, int _code, int _num, MonsterInfo*& _info);
	// 가장 가까운 유저는 누구인가
	bool GetNearestChracter(User* _user, int _monstercode, int _monsternum, float& _length, Character*& _character);
	// 몬스터 이동 정보 및 공격 판단
	bool Monster_Req_Move(User* _user, char* _buf);
	// 보스 몬스터 이동 정보 및 공격 판단
	bool BossMonster_Req_Move(User* _user, char* _buf);

	// 스레드 함수(몬스터 스폰)
	static DWORD WINAPI MonsterSpawnTimerProcess(LPVOID _user);

public:
	static void CreateInstance();
	static InGameManager* GetInstance();
	static void DestroyInstance();

	bool MangerInitialize();
	void EndManager();
	// 인게임 요청 처리
	RESULT	InGame_Init_Packet(User* _user);
	// 인게임 상태에서 강종했을때 뒷처리
	bool User_InGame_Compulsion_Exit(User* _user);
	// 채널 들어가기
	bool User_Enter_Channel(User* _user);
	// 특정 채널 들어가기
	bool User_Enter_Channel(User* _user, int _channelnum);
	// 채널 나가기
	void User_Leave_Channel(User* _user);
	// 특정 채널 나가기
	bool User_Leave_Channel(User* _user, int _channelnum);

	// 파티방 생성
	bool User_Create_PartyRoom(User* _user);
	// 파티방 삭제
	bool User_Remove_PartyRoom(User* _user);
	// 파티방 삭제(코드)
	bool User_Remove_PartyRoom(char* _code);
	// 파티에 유저 추가(참여하는유저코드,파티방번호)
	bool User_Party_AddUser(User* _user, int _partyroomnum);
	// 해당 유저 파티방 인원수
	int User_PartyRoom_UserNum(User* _user);
	//  파티방에 혼자인가
	bool User_PartyRoom_Alone(int _partyroomnum);
	// 파티 유저 강퇴
	bool User_PartyRoom_User_Kick(User* _user, char* _code);
	// 파티 유저 탈퇴
	bool User_PartRoom_User_Leave(User* _user);
	// 파티장 위임
	bool User_PartyRoom_Leader_Delegate(User* _user, char* _code);
	// 던전 채널 들어가기
	void User_EnterInDun_Channel(User* _user);
	// 던전 채널 나가기
	void User_LeaveInDun_Channel(User* _user);
	// 스테이지 입장 준비 (파티원)
	void User_Stage_Ready(User* _user);

	// 파티방 전달
	PartyRoom* GetPartyRoomSearch(User* _user);
	
};

#endif
