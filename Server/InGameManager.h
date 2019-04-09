#ifndef _INGAME_MANAGER_H_
#define _INGAME_MANAGER_H_

#include "Global.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"
#include "UserManager.h"
#include "DBManager.h"
#include "CharacterVerification.h"
#include "CriticalSectionManager.h"
#include "ChannelSystem.h"

class InGameManager
{
private:
	static InGameManager* Instance;

	InGameManager();
	~InGameManager();

	CharacterVerification* verification;
	ChannelSystem*	channelsystem;
	PartySystem* m_partysystem;

	// **Pack 함수** //
	// 다른 유저 위치정보 요청. 유저숫자,코드,직업코드,닉네임,위치,회전
	void User_Pack_OtherUserPosData(User * _user);
	// 접속한 유저 정보 패킹. 코드,직업코드,닉네임,위치,회전
	void User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize);
	// 이동 요청 
	bool User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize);
	// 이동 시작
	bool User_Pack_MoveStart(User * _user, char * _buf, int & _datasize, char* _relesedata, int& _rdatasize);
	//  회전 정보. 코드,회전값(vector3)
	void User_Pack_Rotation(User*_user, char* _data, int& _datasize);
	// 채널 정보 전송. 채널번호,채널유저수....x6
	void User_Pack_ChannelInfo(User* _user);
	// 채널 이동 결과 패킹. 성공실패,채널번호
	bool User_Pack_ChannelChangeResult(User* _user, char* _buf, int _choice, int& _datasize, int& _oldchannelnum);
	// 현재 접속한 채널몇번이지 패킹
	void User_Pack_Current_ChannelInfo(User* _user, char* _buf, int& _datasize);
	// 특정한 유저에게 초대 요청 패킹(파티방번호,유저코드,유저닉네임)

	// 파티 초대 결과 패킹. 결과
	void User_Pack_Party_Invite_Result(User * _user, bool _result, char* _data, int& _datasize);
	// 파티 강퇴 결과 패킹.결과,(성공이면 코드 실패면 끝)
	void User_Pack_Party_Kick_Result(User* _user, bool _result, char* _code, char* _data, int& _datasize);
	// 파티 탈퇴 결과 패킹.결과
	void User_Pack_Party_Leave_Result(User* _user, bool _result, char* _data, int& _datasize);
	// 파티 리더 위임 결과 패킹.결과,(성공이면 코드 실패면 끝)
	void User_Pack_Party_Leader_Delegate_Result(User* _user, bool _result, char* _code, char* _data, int& _datasize);
	
	// 던전 퇴장시 채널정보 패킹. 채널번호

	// **UnPack 함수**
	// 캐릭터 정보
	void User_Unpack_CharacterInfo(User* _user, char* _buf);
	// 채널 이동 요청
	void User_UnPack_ChannelChange(User* _user, char* _buf, int& _channelnum);
	// 초대 요청 언팩 (유저 캐릭터 코드)
	void User_Unpack_PartyRoom_Invite(User* _user, char* _buf, char* _code);
	// 초대 요청 결과
	void User_Unpack_PartyRoom_Invite_Result(User* _user, char*_buf, bool& _result, char* _code, int& _partyroomnum);
	// 파티원 강퇴 요청
	void User_Unpack_PartyRoom_Kick(User* _user, char* _buf, char* _code);
	// 파티 리더 위임
	void User_Unpack_PartyRoom_Leader_Delegate(User* _user, char* _buf, char* _code);

	// ** 다른유저에게 줄 정보 Pack ** //
	// 이동이나 회전 정보 Vecotr3. 
	void User_Pack_MoveInfoToOther(User* _user, char* _data, int& _datasize);
	// 유저코드 (특정유저가 게임에서 나가면)
	void User_Pack_Leave_InGame(User* _user, char* _data, int& _datasize);
	// 파티 초대
	void User_Pack_Party_InviteToOther(User* _user, char* _data, int& _datasize);
	// 파티 유저 캐릭터 정보 pack
	void User_Pack_Party_CharacterInfoToOther(User* _user, char* _data, int& _datasize);
	// 파티 초대했는데 방에 못들어갔을때. 결과
	void User_Pack_Party_InviteResultToOther(User* _user, char* _data, int& _datasize);
	// 파티 강퇴당했다는 정보 패킹(사실 아무것도없다). 프로토콜
	void User_Pack_Party_Kicked(User* _user, char* _data, int& _datasize);
	// 파티 강퇴 당한 유저 정보 패킹. 코드
	void User_Pack_Party_KickInfo(User* _user, char* _data, int& _datasize, char*_code);
	// 파티방 터졌을때 파티유저에게 터졌다는 정보 패킹. 프로토콜만
	void User_Pack_PartyRoom_Leader_Leave(User* _user, char* _data, int& _datasize);
	// 파티 탈퇴한 유저 정보 패킹
	//void User_Pack_Party_LeaveInfo(User* _user, char* _data, int& _datasize);
	// 파티 리더 바뀐 유저 정보 패킷. 코드,코드
	void User_Pack_PartyRoom_Leader_Delegate(User* _user, char* _data, int& _datasize, char* _code);

	// 현재 캐릭터 DB에 저장
	void User_CurCharacter_Save(User* _user);

	// 해당 유저가 파티에 속해있는지
	bool User_IsParty(char* _code);

	// **Send 함수** //
	// 다른 유저 이동정보 전송
	void User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);
	// 다른 유저 인게임에서 떠난 정보 전송
	void User_Send_LeaveInfoToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);
	// 다른 유저 인게임에서 떠난 정보 채널에 전송
	void User_Send_Channel_LeaveInfoToOther(User* _user, PROTOCOL _p, int _channelnum, char* _data, int& _datasize);
	// 다른 유저 인게임에서 떠난 정보 채널에 전송(파티원 제외)
	void User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(User* _user, PROTOCOL _p, int _channelnum, char* _data, int& _datasize);
	// 특정 유저(code) 파티 초대 전송
	void User_Send_Party_InviteToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize, char* _code);
	// 파티원에게 전송
	void User_Send_Party_ToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize);
	// 던전에 들어갔을때 채널에 속해있는 유저들한테 전송
	void User_Send_Party_Eneter_Dungeon(User* _user, PROTOCOL _p);
	// 던전에 나갔을때 채널에 속해있는 유저들한테 전송
	void User_Send_Party_Leave_Dungeon(User* _user, PROTOCOL _p);
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
	// 파티 초대
	//bool User_Party_Invite(User* _user,char* _code);	
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
};

#endif
