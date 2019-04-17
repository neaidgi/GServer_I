#include "InGameManager.h"

InGameManager* InGameManager::Instance = nullptr;

InGameManager::InGameManager()
{
	verification = nullptr;
	channelsystem = nullptr;
	m_partysystem = nullptr;
}

InGameManager::~InGameManager()
{
	delete verification;
	delete channelsystem;
	delete m_partysystem;
}

void InGameManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new InGameManager();
	}
}

InGameManager* InGameManager::GetInstance()
{
	return Instance;
}

void InGameManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool InGameManager::MangerInitialize()
{
	// 인증, 채널시스템,파티시스템 초기화
	verification = new CharacterVerification();
	channelsystem = new ChannelSystem();
	m_partysystem = new PartySystem();

	channelsystem->InitializeChannel();
	m_partysystem->InitializePartySystem();
	return true;
}

// 서버 종료
void InGameManager::EndManager()
{

}

// 접속한 유저리스트 보냄
void InGameManager::User_Pack_OtherUserPosData(User * _user)
{
	// 인게임에 접속중인 유저 리스트(카운트, 직업 코드, 닉네임, 위치...rep)
	// 
	// count자리 비워두기. 나중에 count 넣어줄때는 ptr 사용하기
	PROTOCOL sendprotocol;
	char data[BUFSIZE];
	char* ptr = data;
	int size = 0;

	// 카운트 자리 남기기
	ptr += sizeof(int);

	int usercount = 0;
	int character_jobcode;
	int nicksize = 0;
	int codesize = 0;

	User* user_temp = nullptr;
	Character* character_temp;

	CriticalSectionManager::GetInstance()->Enter();

	// 채널에 들어와있는 유저 정보 패킹
	channelsystem->StartSearchTownUser(_user->GetChannelNum());

	while (1)
	{
		// 유저 검색
		if (channelsystem->SearchTownUser(_user->GetChannelNum(), user_temp) == true)
		{
			if (_user->getSocket() != user_temp->getSocket() && user_temp->isIngame() == true)
			{
				character_temp = user_temp->GetCurCharacter();
				character_jobcode = character_temp->GetCharacter_JobCode();
				nicksize = strlen(character_temp->GetCharacter_Name());
				codesize = strlen(character_temp->GetCharacter_Code());

				// 코드 사이즈
				memcpy(ptr, &codesize, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);

				// 코드
				memcpy(ptr, character_temp->GetCharacter_Code(), codesize);
				ptr += codesize;
				size += codesize;

				// 직업 코드
				memcpy(ptr, &character_jobcode, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);
				// 닉네임 사이즈
				memcpy(ptr, &nicksize, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);
				// 닉네임
				memcpy(ptr, character_temp->GetCharacter_Name(), nicksize);
				ptr += nicksize;
				size += nicksize;
				// 위치
				memcpy(ptr, &character_temp->GetPosition(), sizeof(Vector3));
				ptr += sizeof(Vector3);
				size += sizeof(Vector3);
				// 회전
				memcpy(ptr, &character_temp->GetRotation(), sizeof(Vector3));
				ptr += sizeof(Vector3);
				size += sizeof(Vector3);

				usercount++;
			}
		}
		else
		{
			break;
		}
	}

	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "유저리스트 전송 %d명", usercount);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	memcpy(data, &usercount, sizeof(int));
	size += sizeof(int);

	sendprotocol = SERVER_INGAME_OTHERPLAYERLIST_RESULT;
	_user->Quepack(sendprotocol, data, size);
	CriticalSectionManager::GetInstance()->Leave();
}

// 접속한 유저 정보 패킹
void InGameManager::User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize)
{
	PROTOCOL sendprotocol;
	char* ptr = _data;
	int size = 0;
	int len = 0;

	memset(ptr, 0, BUFSIZE);

	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	// 코드 사이즈
	len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	sprintf(msg, "접속한 유저 데이터 :: 코드사이즈: %d ", len);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 코드
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), len);
	ptr += len;
	size += len;

	sprintf(msg, "코드: [%s] ", _user->GetCurCharacter()->GetCharacter_Code());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 직업 
	int jobcode = _user->GetCurCharacter()->GetCharacter_JobCode();
	memcpy(ptr, &jobcode, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	sprintf(msg, "직업코드: [%d] ", jobcode);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 닉네임 사이즈
	len = strlen(_user->GetCurCharacter()->GetCharacter_Name());
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	sprintf(msg, "닉네임사이즈: [%d] ", len);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 닉네임
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Name(), len);
	ptr += len;
	size += len;

	sprintf(msg, "닉네임: [%s]\n", _user->GetCurCharacter()->GetCharacter_Name());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 위치
	memcpy(ptr, &_user->GetCurCharacter()->GetPosition(), sizeof(Vector3));
	ptr += sizeof(Vector3);
	size += sizeof(Vector3);

	sprintf(msg, "위치: [%f] [%f] [%f] ", _user->GetCurCharacter()->GetPosition().x, _user->GetCurCharacter()->GetPosition().y, _user->GetCurCharacter()->GetPosition().z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 회전
	memcpy(ptr, &_user->GetCurCharacter()->GetRotation(), sizeof(Vector3));
	ptr += sizeof(Vector3);
	size += sizeof(Vector3);

	sprintf(msg, "회전: [%f] [%f] [%f] ", _user->GetCurCharacter()->GetRotation().x, _user->GetCurCharacter()->GetRotation().y, _user->GetCurCharacter()->GetRotation().z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = size;
}

// 이동 완료 요청
bool InGameManager::User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize)
{
	Vector3 prePos;
	Vector3 curPos;
	float dirx, diry;
	bool lesult;
	int datasize = 0;
	int len = 0;
	char* ptr = _buf;
	char msg[BUFSIZE];

	// 받은 데이터 복사
	prePos = _user->GetCurCharacter()->GetPosition();

	// 위치 복사
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "이동 완료 데이터 :: 아이디: [%s] 위치: [%f] [%f] [%f]", _user->getID(), curPos.x, curPos.y,
		curPos.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 버퍼 클리어
	ptr = _buf;
	memset(_buf, 0, sizeof(_buf));

	// 이동검증
	//
	//
	lesult = true;

	// 결과
	memcpy(ptr, &lesult, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// 비정상이동
	if (lesult == false)
	{
		// 위치
		memcpy(ptr, &prePos, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr += sizeof(Vector3);
	}
	else	 // 정상이동
	{
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// 다른유저에게 줄 정보변수
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// 다른유저에게 줄 정보 데이터
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize);

	// 외부에 복사
	memcpy(_releasedata, relesedata, rdatasize);
	_rdatasize = rdatasize;

	return lesult;
}

// 이동 시작
bool InGameManager::User_Pack_MoveStart(User * _user, char * _buf, int & _datasize, char* _releasedata, int& _rdatasize)
{
	Vector3 prePos;
	Vector3 curPos;
	Vector3 curRot;
	float dirx, diry;
	bool lesult;
	int datasize = 0;
	int len = 0;
	char* ptr = _buf;
	char msg[BUFSIZE];

	// 이전 위치 복사
	prePos = _user->GetCurCharacter()->GetPosition();

	// 현재 위치 복사
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	memcpy(&curRot, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "이동 완료 데이터 :: 아이디: [%s] 위치: [%f] [%f] [%f]", _user->getID(), curPos.x, curPos.y,
		curPos.z, curRot.x, curRot.y, curRot.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 초기화
	ptr = _buf;
	memset(_buf, 0, sizeof(_buf));

	// 이동검증
	//
	//
	lesult = true;

	// 결과
	memcpy(ptr, &lesult, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// 비정상이동
	if (lesult == false)
	{
		// 위치
		memcpy(ptr, &prePos, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr += sizeof(Vector3);

		// 회전
		memcpy(ptr, &curRot, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr += sizeof(Vector3);

		// 방향 x
		memcpy(ptr, &dirx, sizeof(float));
		datasize += sizeof(float);
		ptr += sizeof(float);

		// 방향 y
		memcpy(ptr, &diry, sizeof(float));
		datasize += sizeof(float);
		ptr += sizeof(float);

		_user->GetCurCharacter()->SetRotation(curRot);
	}
	else	 // 정상이동
	{
		_user->GetCurCharacter()->SetRotation(curRot);
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// 다른유저에게 줄 정보변수
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// 다른유저에게 줄 정보 데이터
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize);

	// 외부에 복사
	memcpy(_releasedata, relesedata, rdatasize);
	_rdatasize = rdatasize;

	return lesult;
}

// 유저 회전 패킹
void InGameManager::User_Pack_Rotation(User * _user, char * _data, int & _datasize)
{
	Vector3 curRot;
	char * ptr = _data;
	int datasize = 0;
	int len = 0;

	memcpy(&curRot, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	_user->GetCurCharacter()->SetRotation(curRot);

	memset(_data, 0, BUFSIZE);
	ptr = _data;

	// 코드 사이즈
	len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	datasize += sizeof(int);
	// 코드
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), len);
	ptr += len;
	datasize += len;
	// 회전값
	memcpy(ptr, &curRot, sizeof(Vector3));
	ptr += sizeof(Vector3);
	datasize += sizeof(Vector3);

	_datasize = datasize;

	// 메세지
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "회전 데이터 :: 아이디: [%s] 회전: [%f] [%f] [%f]", _user->getID(), curRot.x, curRot.y,
		curRot.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

}

// 유저 코드 패킹
void InGameManager::User_Pack_UserCode(User * _user, char * _data, int & _datasize)
{
	int datasize = 0;
	int len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// 코드 사이즈
	memcpy(ptr, &len, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 코드
	memcpy(ptr, curcharacter->GetCharacter_Code(), len);
	datasize += len;
	ptr += len;

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_UserCode :: 떠나는 유저의 캐릭터 코드 : [%s] ", curcharacter->GetCharacter_Code());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 채널 정보 전송
void InGameManager::User_Pack_ChannelInfo(User * _user)
{
	PROTOCOL sendprotocol;
	char data[BUFSIZE];
	char* ptr = data;
	int size = 0;
	int usercount = 0;

	char msg[BUFSIZE];

	CriticalSectionManager::GetInstance()->Enter();

	for (int i = 0; i < 6; i++)
	{
		// 채널 번호
		memcpy(ptr, &i, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// 채널에 유저수
		usercount = channelsystem->GetChannelSize(i);

		memcpy(ptr, &usercount, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		memset(msg, 0, sizeof(msg));
		sprintf(msg, "채널 정보 전달 채널번호 : [%d] 유저수 : [%d]", i, usercount);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	sendprotocol = SERVER_INGAME_CHANNLE_INFO_RESULT;
	_user->Quepack(sendprotocol, data, size);

	CriticalSectionManager::GetInstance()->Leave();
}

// 채널 이동 결과 패킹
bool InGameManager::User_Pack_ChannelChangeResult(User * _user, char* _buf, int _choice, int& _datasize, int& _oldchannelnum)
{
	int oldchannelnum = 0;
	bool result;
	int size = 0;
	char* ptr = _buf;

	char msg[BUFSIZE];

	// 채널 번호 임시저장
	oldchannelnum = _user->GetChannelNum();

	memset(_buf, 0, BUFSIZE);
	ptr = _buf;

	CriticalSectionManager::GetInstance()->Enter();

	if (User_Enter_Channel(_user, _choice))
	{
		result = true;
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "채널 이동성공 채널번호 : [%d] ", _choice);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		// 그전에 채널에서 나가기
		User_Leave_Channel(_user, oldchannelnum);

		memcpy(ptr, &result, sizeof(bool));
		ptr += sizeof(bool);
		size += sizeof(bool);

		memcpy(ptr, &_choice, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);
	}
	else
	{
		_user->SetChannelNum(oldchannelnum);
		result = false;

		memset(msg, 0, sizeof(msg));
		sprintf(msg, "채널 이동실패 채널번호 : [%d] ", _choice);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		memcpy(ptr, &result, sizeof(bool));
		ptr += sizeof(bool);
		size += sizeof(bool);
	}

	_datasize = size;
	_oldchannelnum = oldchannelnum;

	CriticalSectionManager::GetInstance()->Leave();

	return result;
}

// 현재 접속한 채널몇번이지 패킹
void InGameManager::User_Pack_Current_ChannelInfo(User * _user, char * _buf, int & _datasize)
{
	int channelnum = 0;
	int size = 0;
	char* ptr = _buf;

	channelnum = _user->GetChannelNum();

	memcpy(ptr, &channelnum, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	_datasize = size;
}

// 파티 초대 패킹(코드,닉네임,파티번호)
void InGameManager::User_Pack_Party_InviteToOther(User * _user, char * _data, int & _datasize)
{
	//CriticalSectionManager::GetInstance()->Enter();

	int datasize = 0;
	int partynum = _user->GetPartyNum();
	int codelen = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	int nicklen = strlen(_user->GetCurCharacter()->GetCharacter_Name());
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// 코드 사이즈
	memcpy(ptr, &codelen, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 코드
	memcpy(ptr, curcharacter->GetCharacter_Code(), codelen);
	datasize += codelen;
	ptr += codelen;

	// 닉네임 사이즈
	memcpy(ptr, &nicklen, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 닉네임
	memcpy(ptr, curcharacter->GetCharacter_Name(), nicklen);
	datasize += nicklen;
	ptr += nicklen;

	// 파티 번호
	memcpy(ptr, &partynum, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_Invite_Other :: 파티번호 : [%d] 초대하는 유저 코드 : [%s] 초대하는 유저 닉네임 : [%s] ", _user->GetPartyNum(), curcharacter->GetCharacter_Code(), curcharacter->GetCharacter_Name());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;

	//CriticalSectionManager::GetInstance()->Leave();
}

// 파티에 속해있는 유저정보 패킹(파티번호,유저숫자,코드,직업코드,닉네임,Hp,Mp,방장인지)
void InGameManager::User_Pack_Party_CharacterInfoToOther(User * _user, char * _data, int & _datasize)
{
	char* ptr = _data;

	int datasize = 0;
	int partynum = _user->GetPartyNum();
	int partyusercount = 0;
	int codelen = 0;
	int nicklen = 0;
	int character_jobcode;
	bool leader = false;
	float character_hp = 0;
	float character_mp = 0;
	PartyRoom* partyroom = nullptr;

	char msg[BUFSIZE];

	User* user_temp = nullptr;
	Character* character_temp;

	// 파티방
	partyroom = m_partysystem->GetPartyRoomSearch(partynum);
	partyusercount = m_partysystem->GetPartyRoomUserNum(partynum);

	// 파티 번호
	memcpy(ptr, &partynum, sizeof(int));
	ptr += sizeof(int);
	datasize += sizeof(int);

	// 유저 숫자
	memcpy(ptr, &partyusercount, sizeof(int));
	ptr += sizeof(int);
	datasize += sizeof(int);

	CriticalSectionManager::GetInstance()->Enter();

	// 파티방 검색 시작
	partyroom->StartSearchPartyRoom();

	while (1)
	{
		// 유저 검색
		if (partyroom->SearchPartyRoom(user_temp) == true)
		{
			if (user_temp->isIngame() == true && user_temp->isParty() == true)
			{
				character_temp = user_temp->GetCurCharacter();
				character_jobcode = character_temp->GetCharacter_JobCode();
				nicklen = strlen(character_temp->GetCharacter_Name());
				codelen = strlen(character_temp->GetCharacter_Code());
				leader = user_temp->isPartyLeader();
				character_hp = character_temp->GetCharacter_Health();
				character_mp = character_temp->GetCharacter_Mana();

				// 코드 사이즈
				memcpy(ptr, &codelen, sizeof(int));
				ptr += sizeof(int);
				datasize += sizeof(int);

				// 코드
				memcpy(ptr, character_temp->GetCharacter_Code(), codelen);
				ptr += codelen;
				datasize += codelen;

				// 직업 코드
				memcpy(ptr, &character_jobcode, sizeof(int));
				ptr += sizeof(int);
				datasize += sizeof(int);

				// 닉네임 사이즈
				memcpy(ptr, &nicklen, sizeof(int));
				ptr += sizeof(int);
				datasize += sizeof(int);

				// 닉네임
				memcpy(ptr, character_temp->GetCharacter_Name(), nicklen);
				ptr += nicklen;
				datasize += nicklen;

				// HP
				memcpy(ptr, &character_hp, sizeof(float));
				ptr += sizeof(float);
				datasize += sizeof(float);

				// MP
				memcpy(ptr, &character_mp, sizeof(float));
				ptr += sizeof(float);
				datasize += sizeof(float);

				// 방장인지
				memcpy(ptr, &leader, sizeof(bool));
				ptr += sizeof(bool);
				datasize += sizeof(bool);
			}
		}
		else
		{
			break;
		}
	}

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_InviteResultToOther :: 파티번호 : [%d] 파티인원 [%d]", _user->GetPartyNum(), partyusercount);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;

	CriticalSectionManager::GetInstance()->Leave();
}

// 파티 초대했는데 방에 못들어갔을때
void InGameManager::User_Pack_Party_InviteResultToOther(User * _user, char * _data, int & _datasize)
{
	int datasize = 0;
	char* ptr = _data;
	char msg[BUFSIZE];
	bool result = false;

	// 결과
	memcpy(ptr, &result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_InviteResultToOther ::  ");
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 파티 프로토콜 패킹 (프로토콜)
void InGameManager::User_Pack_Party_Protocol(User * _user, char * _data, int & _datasize)
{
	int datasize = 0;
	char* ptr = _data;

	char msg[BUFSIZE];

	_datasize = datasize;
}

// 파티 강퇴 당한 유저 정보 패킹. (특정 유저의 코드)
void InGameManager::User_Pack_Party_KickInfo(User * _user, char * _data, int & _datasize, char * _code)
{
	int datasize = 0;
	int len = strlen(_code);
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// 코드 사이즈
	memcpy(ptr, &len, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 코드
	memcpy(ptr, _code, len);
	datasize += len;
	ptr += len;

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_KickInfo :: 강퇴당한 유저의 캐릭터 코드 : [%s] ", _code);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 파티 리더 바뀐 유저 정보 패킷. (코드, 코드)
void InGameManager::User_Pack_PartyRoom_Leader_Delegate(User * _user, char * _data, int & _datasize, char * _code)
{
	int datasize = 0;
	int oldlne = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	int len = strlen(_code);
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// 전 파티 리더 코드 사이즈
	memcpy(ptr, &oldlne, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 전 파티 리더 코드
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), oldlne);
	datasize += oldlne;
	ptr += oldlne;

	// 현 파티 리더 코드 사이즈
	memcpy(ptr, &len, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 현 파티 리더 코드
	memcpy(ptr, _code, len);
	datasize += len;
	ptr += len;

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_PartyRoom_Leader_Delegate :: 새로운 파티 리더 유저의 캐릭터 코드 : [%s] ", _code);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 던전 입장시 스폰위치. (파티원수,유저코드,좌표)
void InGameManager::User_Pack_Party_Dungeon_SpawnData(User * _user, char * _data, int & _datasize)
{
	char* ptr = _data;
	int size = 0;

	int usercount = 0;
	int codesize = 0;

	PartyRoom* partyroom_temp = nullptr;
	User* user_temp = nullptr;
	Character* character_temp = nullptr;

	int count = 0;
	Vector3 pos;
	Vector3 spawnpos[DUNGEON_SPAWNPOS_MAXCOUNT];

	GameDataManager::GetInstance()->Dungeon_SpawnPos_Vecotr(spawnpos);

	CriticalSectionManager::GetInstance()->Enter();

	// 채널에 들어와있는 유저 정보 패킹
	partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());
	
	usercount = partyroom_temp->GetPartRoomUserNum();

	// 유저 숫자
	memcpy(ptr, &usercount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 검색초기화
	partyroom_temp->StartSearchPartyRoom();

	while (partyroom_temp->SearchPartyRoom(user_temp))
	{
		character_temp = user_temp->GetCurCharacter();
		codesize = strlen(character_temp->GetCharacter_Code());
		pos = spawnpos[count];

		memcpy(ptr, &codesize, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		memcpy(ptr, character_temp->GetCharacter_Code(), codesize);
		ptr += codesize;
		size += codesize;

		// 스폰지역
		memcpy(ptr, &pos, sizeof(Vector3));
		ptr += sizeof(Vector3);
		size += sizeof(Vector3);

		count++;
	}

	_datasize = size;

	CriticalSectionManager::GetInstance()->Leave();
}

// 던전 스테이지 입장시 스폰위치. (파티원수,유저코드,좌표)
void InGameManager::User_Pack_Party_Dungeon_Stage_SpawnData(User * _user, char * _data, int & _datasize)
{
	char* ptr = _data;
	int size = 0;

	int usercount = 0;
	int codesize = 0;

	PartyRoom* partyroom_temp = nullptr;
	User* user_temp = nullptr;
	Character* character_temp = nullptr;

	int count = 0;
	Vector3 pos;
	Vector3 spawnpos[DUNGEON_SPAWNPOS_MAXCOUNT];

	GameDataManager::GetInstance()->Dungeon_Stage_SpawnPos_Vecotr(spawnpos);

	CriticalSectionManager::GetInstance()->Enter();

	// 채널에 들어와있는 유저 정보 패킹
	partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	usercount = partyroom_temp->GetPartRoomUserNum();

	// 유저 숫자
	memcpy(ptr, &usercount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 검색초기화
	partyroom_temp->StartSearchPartyRoom();

	while (partyroom_temp->SearchPartyRoom(user_temp))
	{
		character_temp = user_temp->GetCurCharacter();
		codesize = strlen(character_temp->GetCharacter_Code());
		pos = spawnpos[count];

		memcpy(ptr, &codesize, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		memcpy(ptr, character_temp->GetCharacter_Code(), codesize);
		ptr += codesize;
		size += codesize;

		// 스폰지역
		memcpy(ptr, &pos, sizeof(Vector3));
		ptr += sizeof(Vector3);
		size += sizeof(Vector3);

		count++;
	}

	_datasize = size;

	CriticalSectionManager::GetInstance()->Leave();
}

// 던전 스테이지 입장시 몬스터 정보.
void InGameManager::User_Pack_Dungeon_Monster_SpawnInfo(User * _user, char * _data, int & _datasize)
{
	//char* ptr = _data;
	//int size = 0;

	//int monstercount = 0;

	//PartyRoom* partyroom_temp = nullptr;

	//int count = 0;
	//Vector3 pos;
	//Vector3 spawnpos[DUNGEON_SPAWNPOS_MAXCOUNT];

	//GameDataManager::GetInstance()->Dungeon_Stage_SpawnPos_Vecotr(spawnpos);

	//CriticalSectionManager::GetInstance()->Enter();

	//// 파티방
	//partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	//// 유저 숫자
	//memcpy(ptr, &usercount, sizeof(int));
	//ptr += sizeof(int);
	//size += sizeof(int);

	//// 검색초기화
	//partyroom_temp->StartSearchPartyRoom();

	//while (partyroom_temp->SearchPartyRoom(user_temp))
	//{
	//	character_temp = user_temp->GetCurCharacter();
	//	codesize = strlen(character_temp->GetCharacter_Code());
	//	pos = spawnpos[count];

	//	memcpy(ptr, &codesize, sizeof(int));
	//	ptr += sizeof(int);
	//	size += sizeof(int);

	//	memcpy(ptr, character_temp->GetCharacter_Code(), codesize);
	//	ptr += codesize;
	//	size += codesize;

	//	// 스폰지역
	//	memcpy(ptr, &pos, sizeof(Vector3));
	//	ptr += sizeof(Vector3);
	//	size += sizeof(Vector3);

	//	count++;
	//}

	//_datasize = size;

	//CriticalSectionManager::GetInstance()->Leave();
}

// 파티 초대 결과 패킹
void InGameManager::User_Pack_Party_Result(User * _user, bool _result, char * _data, int & _datasize)
{
	int datasize = 0;
	char* ptr = _data;
	char msg[BUFSIZE];
	bool result = _result;

	// 결과
	memcpy(ptr, &result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// 메세지
	memset(msg, 0, sizeof(msg));

	if (result)
	{
		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [성공] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [실패] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	_datasize = datasize;
}

// 파티 강퇴 결과 패킹
void InGameManager::User_Pack_Party_Result_Code(User * _user, bool _result, char * _code, char * _data, int & _datasize)
{
	int datasize = 0;

	char* ptr = _data;
	char msg[BUFSIZE];
	int len = strlen(_code);
	bool result = _result;

	// 결과
	memcpy(ptr, &result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// 메세지
	memset(msg, 0, sizeof(msg));

	if (result)
	{
		// 코드 길이
		memcpy(ptr, &len, sizeof(int));
		datasize += sizeof(int);
		ptr += sizeof(int);

		// 코드
		memcpy(ptr, _code, len);
		datasize += len;
		ptr += len;

		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [성공] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [실패] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	_datasize = datasize;
}

// 던전 입장시 스폰 지역 패킹
void InGameManager::User_Pack_Dungeon_SpawnData(User * _user, int _count, char * _data, int & _datasize)
{
	char* ptr = _data;
	int size = 0;
	int len = 0;

	int count = 0;
	Vector3 pos;
	Vector3 spawnpos[DUNGEON_SPAWNPOS_MAXCOUNT];

	GameDataManager::GetInstance()->Dungeon_SpawnPos_Vecotr(spawnpos);
	pos = spawnpos[_count];

	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	// 위치
	memcpy(ptr, &pos, sizeof(Vector3));
	ptr += sizeof(Vector3);
	size += sizeof(Vector3);
	
	_datasize = size;
}

// 채널 이동 요청 언팩
void InGameManager::User_UnPack_ChannelChange(User * _user, char * _buf, int& _channelnum)
{
	int channelnum = 0;

	memcpy(&channelnum, _buf, sizeof(int));

	_channelnum = channelnum;
}

// 캐릭터 코드 언팩 (코드)
void InGameManager::User_Unpack_UserCode(User * _user, char * _buf, char * _code)
{
	char* ptr = _buf;
	int len = 0;
	// 코드 길이
	memcpy(&len, ptr, sizeof(int));
	ptr += sizeof(int);
	// 코드
	memcpy(_code, ptr, len);
	ptr += len;
}

// 초대 요청 결과 언팩
void InGameManager::User_Unpack_PartyRoom_Invite_Result(User * _user, char * _buf, bool& _result, char * _code, int & _partyroomnum)
{
	char* ptr = _buf;
	int len = 0;
	bool result = false;
	int partyroomnum;

	// 결과
	memcpy(&result, ptr, sizeof(bool));
	ptr += sizeof(bool);
	// 코드 길이
	memcpy(&len, ptr, sizeof(int));
	ptr += sizeof(int);
	// 코드
	memcpy(_code, ptr, len);
	ptr += len;
	// 파티 번호
	memcpy(&partyroomnum, ptr, sizeof(int));
	ptr += sizeof(int);

	_result = result;
	_partyroomnum = partyroomnum;
}

// 이동이나 회전 정보 Vecotr3. 
void InGameManager::User_Pack_MoveInfoToOther(User* _user, char * _data, int & _datasize)
{
	int datasize = 0;
	int len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// 코드 사이즈
	memcpy(ptr, &len, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// 코드
	memcpy(ptr, curcharacter->GetCharacter_Code(), len);
	datasize += len;
	ptr += len;

	// 위치
	memcpy(ptr, &curcharacter->GetPosition(), sizeof(Vector3));
	datasize += sizeof(Vector3);
	ptr += sizeof(Vector3);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_MoveInfoToOther :: 코드 : [%s] 위치: [%f] [%f] [%f]", curcharacter->GetCharacter_Code(), curcharacter->GetPosition().x, curcharacter->GetPosition().y,
		curcharacter->GetPosition().z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 현재 캐릭터 DB에 저장
void InGameManager::User_CurCharacter_Save(User * _user)
{
	CriticalSectionManager::GetInstance()->Enter();
	char msg[BUFSIZE];

	if (DBManager::GetInstance()->Character_Save(_user->getID(), _user->GetCurCharacter()) == false)
	{
		// 메세지
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "User_CurCharacter_Save :: 실패, ID : [%s]", _user->getID());
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		// 메세지
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "User_CurCharacter_Save :: 성공, ID : [%s]", _user->getID());
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// 해당유저(코드로검색)가 파티에 속해있는가
bool InGameManager::User_IsParty(char * _code)
{
	User* user = nullptr;

	user = UserManager::GetInstance()->getUserCode(_code);
	if (user == nullptr)
	{
		return false;
	}

	if (user->isParty() == false)
	{
		return false;
	}

	return true;
}

// 채널 들어가기
bool InGameManager::User_Enter_Channel(User * _user)
{
	if (channelsystem->ChannelEnter(_user))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 특정 채널 들어가기
bool InGameManager::User_Enter_Channel(User * _user, int _channelnum)
{
	if (channelsystem->ChannelEnter(_user, _channelnum))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 채널 나가기
void InGameManager::User_Leave_Channel(User * _user)
{
	channelsystem->ChannelLeave(_user);
}

// 특정 채널 나가기
bool InGameManager::User_Leave_Channel(User * _user, int _channelnum)
{
	return channelsystem->ChannelLeave(_user, _channelnum);
}

// 파티방 생성
bool InGameManager::User_Create_PartyRoom(User * _user)
{
	return m_partysystem->Party_Create(_user);
}

// 파티방 삭제
bool InGameManager::User_Remove_PartyRoom(User * _user)
{
	return m_partysystem->Party_Remove(_user);
}

// 파티방 삭제 코드
bool InGameManager::User_Remove_PartyRoom(char * _code)
{
	User* user = nullptr;

	user = UserManager::GetInstance()->getUser(_code);

	return m_partysystem->Party_Remove(user);
}

// 파티에 유저 추가(참여하는유저코드,파티방번호)
bool InGameManager::User_Party_AddUser(User* _user, int _partyroomnum)
{
	return m_partysystem->Partyroom_add_User(_user, _partyroomnum);
}

// 해당 유저 파티방 인원수
int InGameManager::User_PartyRoom_UserNum(User * _user)
{
	return m_partysystem->GetPartyRoomUserNum(_user->GetPartyNum());
}

// 파티방에 혼자인가
bool InGameManager::User_PartyRoom_Alone(int _partyroomnum)
{
	int count = 0;
	count = m_partysystem->GetPartyRoomUserNum(_partyroomnum);
	if (count = 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 파티 유저 강퇴
bool InGameManager::User_PartyRoom_User_Kick(User * _user, char * _code)
{
	return m_partysystem->Party_Kick_User(_user, _code);
}

// 파티 유저 탈퇴
bool InGameManager::User_PartRoom_User_Leave(User * _user)
{
	return m_partysystem->Party_User_Leave(_user);;
}

// 파티장 위임
bool InGameManager::User_PartyRoom_Leader_Delegate(User* _user, char* _code)
{
	return m_partysystem->Party_Leader_Delegate(_user, _code);
}

// 던전 채널 들어가기
void InGameManager::User_EnterInDun_Channel(User * _user)
{
	PartyRoom* partyroom = nullptr;
	// 파티 검색
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티 채널추가
	channelsystem->DungeonEnter(partyroom);
}

// 던전 채널 나가기
void InGameManager::User_LeaveInDun_Channel(User * _user)
{
	User* user = nullptr;
	PartyRoom* partyroom = nullptr;
	char buf[BUFSIZE];
	char rdata[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	memset(rdata, 0, sizeof(rdata));
	char* ptr = buf;
	int channelnum = 0;
	int datasize = 0;
	int rdatasize = 0;

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (channelsystem->DungeonLeave(_user->GetPartyNum()) == false)
	{
		// 던전 채널 나가기 실패. 그런 파티가없읍니다.
		return;
	}

	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		// 마을 채널 들어가기(특정채널)
		if (User_Enter_Channel(user, user->GetChannelNum()) == false)
		{
			// 특정채널에 접속못하면 아무채널에나 넣는다
			if (User_Enter_Channel(_user))
			{
				// 채널 입장 성공. 프로토콜은 서버 던전 퇴장
				User_Pack_Current_ChannelInfo(user, buf, datasize);
				user->Quepack(SERVER_INGAME_DUNGEON_LEAVE_RESULT, buf, datasize);

				User_Pack_PlayerPosData(_user, rdata, rdatasize);
				User_Send_MoveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_CONNECT, rdata, rdatasize);
			}
			else
			{
				// 이부분 아직 구현못함. 만약 아무채널에도 접속을 못한다면 어떻게해야할지

			}
		}
		else
		{
			//채널 입장 성공. 프로토콜은 서버 던전 퇴장
			User_Pack_Current_ChannelInfo(user, buf, datasize);
			user->Quepack(SERVER_INGAME_DUNGEON_LEAVE_RESULT, buf, datasize);

			User_Pack_PlayerPosData(_user, rdata, rdatasize);
			User_Send_MoveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_CONNECT, rdata, rdatasize);
		}
	}
}

// 다른 유저 이동정보 전송(채널에 접속해있는 유저들한테 전송)
void InGameManager::User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char * _data, int & _datasize)
{
	User* user = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	channelsystem->StartSearchTownUser(_user->GetChannelNum());

	while (channelsystem->SearchTownUser(_user->GetChannelNum(), user))
	{
		if (user->isIngame() && user->getSocket() != _user->getSocket())
		{
			user->Quepack(_p, _data, _datasize);
			if (user->isSending() == false)
			{
				if (user->TakeOutSendPacket())
				{
					user->IOCP_SendMsg();
				}
			}
			// 메세지
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_MoveInfoToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// 다른 유저 인게임에서 떠난 정보 전송
void InGameManager::User_Send_LeaveInfoToOther(User * _user, PROTOCOL _p, char * _data, int & _datasize)
{
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	char* ptr = buf;
	int datasize = 0;

	User* user = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	// 채널에 속해있으면 채널에 속한 유저한테 보낸다
	if (_user->isChannel())
	{
		User_Send_Channel_LeaveInfoToOther(_user, _p, _user->GetChannelNum(), _data, _datasize);
	}

	// 파티에 속해있으며 파티방장이 아니면 그냥 파티원들한테 보낸다
	if (_user->isParty() && _user->isPartyLeader() == false)
	{
		User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_RESULT, _data, _datasize);
	}// 파티에 속해있으며 파티방장이면 파티방 터트린다.
	else if (_user->isParty() && _user->isPartyLeader())
	{
		User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, buf, datasize);

		// 파티없앤다
		User_Remove_PartyRoom(_user);
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// 채널이동해서 떠났을때 그전에 채널에있는 유저들한테 전송
void InGameManager::User_Send_Channel_LeaveInfoToOther(User * _user, PROTOCOL _p, int _channelnum, char * _data, int & _datasize)
{
	User* user = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	channelsystem->StartSearchTownUser(_channelnum);

	while (channelsystem->SearchTownUser(_channelnum, user))
	{
		if (user->isIngame() && user->getSocket() != _user->getSocket())
		{
			user->Quepack(_p, _data, _datasize);
			if (user->isSending() == false)
			{
				if (user->TakeOutSendPacket())
				{
					user->IOCP_SendMsg();
				}
			}
			// 메세지
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_Channel_LeaveInfoToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// 다른 유저 인게임에서 떠난 정보 채널에 전송(파티원 제외)
void InGameManager::User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(User * _user, PROTOCOL _p, int _channelnum, char * _data, int & _datasize)
{
	User* user = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	channelsystem->StartSearchTownUser(_channelnum);

	while (channelsystem->SearchTownUser(_channelnum, user))
	{
		if (user->isIngame() && user->getSocket() != _user->getSocket() && user->GetPartyNum() != _user->GetPartyNum())
		{
			user->Quepack(_p, _data, _datasize);
			if (user->isSending() == false)
			{
				if (user->TakeOutSendPacket())
				{
					user->IOCP_SendMsg();
				}
			}
			// 메세지
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// 특정 유저(code) 파티 초대 전송
void InGameManager::User_Send_Party_InviteToOther(User* _user, PROTOCOL _p, char* _data, int& _datasize, char* _code)
{
	User* user = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	user = UserManager::GetInstance()->getUserCode(_code);

	if (user->isIngame() && user->getSocket() != _user->getSocket())
	{
		user->Quepack(_p, _data, _datasize);
		if (user->isSending() == false)
		{
			if (user->TakeOutSendPacket())
			{
				user->IOCP_SendMsg();
			}
		}
		// 메세지
		char msg[BUFSIZE];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "User_Send_Party_InviteToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			_p, _datasize, user->GetSendQueueSize());
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// 파티원에게 전송
void InGameManager::User_Send_Party_ToOther(User * _user, PROTOCOL _p, char * _data, int & _datasize)
{
	User* user = nullptr;
	PartyRoom* partyroom = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		if (user->isIngame() && user->getSocket() != _user->getSocket())
		{
			user->Quepack(_p, _data, _datasize);
			if (user->isSending() == false)
			{
				if (user->TakeOutSendPacket())
				{
					user->IOCP_SendMsg();
				}
			}
			// 메세지
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_Party_ToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// 던전에 들어갔을대 채널에 속해있는 유저들한테 전송
void InGameManager::User_Send_Party_Eneter_Dungeon(User * _user, PROTOCOL _p)
{
	PartyRoom* partyroom = nullptr;
	User* user = nullptr;
	char buf[BUFSIZE];
	int datasize = 0;
	memset(buf, 0, sizeof(buf));

	// 유저가 속해있는 파티방을 검색해서 넣는다.
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티방의 각 유저들 채널에 속해있는 유저들한테 나간다고 알려준다.
	// 파티 유저 코드 패킹
	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		// 유저 코드 패킹
		User_Pack_UserCode(user, buf, datasize);

		// 파티 유저가 속한 채널에 send
		User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(user, _p, user->GetChannelNum(), buf, datasize);
	}
}

// 던전에 나갔을때 채널에 속해있는 유저들한테 전송
void InGameManager::User_Send_Party_Leave_Dungeon(User * _user, PROTOCOL _p)
{
	PartyRoom* partyroom = nullptr;
	User* user = nullptr;
	char buf[BUFSIZE];
	int datasize = 0;
	memset(buf, 0, sizeof(buf));

	// 유저가 속해있는 파티방을 검색해서 넣는다.
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티방의 각 유저들 채널에 속해있는 유저들한테 나간다고 알려준다.
	// 파티 유저 코드 패킹
	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		// 유저 코드 패킹
		User_Pack_UserCode(user, buf, datasize);

		// 파티 유저가 속한 채널에 send
		User_Send_Channel_LeaveInfoToOther(user, _p, user->GetChannelNum(), buf, datasize);
	}
}


RESULT InGameManager::InGame_Init_Packet(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char rdata[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	memset(rdata, 0, sizeof(rdata));
	char charactercode[CHARACTERCODESIZE];
	char* ptr = buf;
	bool check;
	int choice = 0;
	int oldchannelnum = 0;
	int partyroomnum = 0;
	int datasize = 0;
	int rdatasize = 0;

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result = RT_DEFAULT;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_INGAME_OTHERPLAYERLIST:		// 유저리스트
		User_Pack_OtherUserPosData(_user);
		User_Pack_PlayerPosData(_user, buf, datasize);
		User_Send_MoveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_CONNECT, buf, datasize);
		result = RT_INGAME_OTHERPLAYER_LIST;
		break;
	case CLIENT_INGAME_MOVE_REPORT:			// 클라 이동
		User_Pack_Move(_user, buf, datasize, rdata, rdatasize);
		User_Send_MoveInfoToOther(_user, SERVER_INGAME_MOVE_OTHERPLAYERINFO, rdata, rdatasize);
		sendprotocol = SERVER_INGAME_MOVE_RESULT;
		_user->Quepack(sendprotocol, buf, datasize);
		result = RT_INGAME_MOVE;
		break;
	case CLIENT_INGAME_MOVE_ROTATION:		// 클라 회전
		User_Pack_Rotation(_user, buf, datasize);
		User_Send_MoveInfoToOther(_user, SERVER_INGAME_MOVE_ROTATION, buf, datasize);
		_user->SetCallback(false);
		result = RT_INGAME_MOVE;
		break;
	case CLIENT_INGAME_CHANNEL_INFO:		// 채널 정보 요청
		User_Pack_ChannelInfo(_user);
		result = RT_INGAME_CHANNEL_INFO;
		break;
	case CLIENT_INGAME_CHANNEL_CHANGE:		// 채널 이동 요청
		User_UnPack_ChannelChange(_user, buf, choice);
		if (User_Pack_ChannelChangeResult(_user, buf, choice, datasize, oldchannelnum))
		{
			// 채널 이동 성공
			User_Pack_UserCode(_user, rdata, rdatasize);
			User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, oldchannelnum, rdata, rdatasize);
		}
		sendprotocol = SERVER_INGAME_CHANNLE_CHANGE_RESULT;
		_user->Quepack(sendprotocol, buf, datasize);
		result = RT_INGAME_CHANNEL_CHANGE;
		break;
	case CLIENT_INGAME_MENU_REQ_CHARACTER:	// 캐릭터 선택화면 요청
		User_Pack_UserCode(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);
		User_Leave_Channel(_user);

		sendprotocol = SERVER_INGAME_MENU_RESULT_CHARACTER;
		_user->Quepack(sendprotocol, buf, 0);
		User_CurCharacter_Save(_user);

		// 현재 접속중인 캐릭터 초기화
		_user->ResetCurCharacter();
		_user->SetLeaveGame();
		_user->SetLeaveDungeon();
		_user->ResetPartyInfo();

		result = RT_INGAME_MENU_CHARACTER;
		break;
	case CLIENT_INGAME_MENU_REQ_LOGOUT:		// 로그아웃 요청
		User_Pack_UserCode(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);
		User_Leave_Channel(_user);

		sendprotocol = SERVER_INGAME_MENU_RESULT_LOGOUT;
		_user->Quepack(sendprotocol, buf, 0);
		User_CurCharacter_Save(_user);

		// 유저 정보 초기화
		_user->ResetUserInfo();
		result = RT_INGAME_MENU_LOGOUT;
		break;
	case CLIENT_INGAME_MENU_EXIT:			// 게임종료 요청
		// 아직 안만듬
		result = RT_INGAME_MENU_EXIT;
		break;
	case CLIENT_INGAME_PARTY_ROOM_INVITE:		// 파티 초대 요청
		memset(charactercode, 0, sizeof(CHARACTERCODESIZE));
		User_Unpack_UserCode(_user, buf, charactercode);

		if (_user->isParty() == false)
		{
			// 파티 초대한 유저가 파티가 없으면 파티방을 만든다.
			User_Create_PartyRoom(_user);
		}

		// 파티에 들어가있지않으면
		if (User_IsParty(charactercode) == false)
		{
			User_Pack_Party_InviteToOther(_user, rdata, rdatasize);
			User_Send_Party_InviteToOther(_user, SERVER_INGAME_PARTY_ROOM_INVITE, rdata, rdatasize, charactercode);
		}
		else
		{
			// 파티초대 실패 패킹하고 보내준다.
			User_Pack_Party_Result(_user, false, buf, datasize);
			sendprotocol = SERVER_INGAME_PARTY_ROOM_INVITE_RESULT;
			_user->Quepack(sendprotocol, buf, datasize);
			// 만들어둔 파티방 삭제
			User_Remove_PartyRoom(_user);
		}
		result = RT_INGAME_PARTY_INVITE;
		break;
	case CLIENT_INGAME_PARTY_ROOM_ANSWER_INVITE:		// 클라 파티 초대 응답
		User_Unpack_PartyRoom_Invite_Result(_user, buf, check, charactercode, partyroomnum);
		if (check) // 초대 승락
		{
			// 파티 유저 추가
			check = User_Party_AddUser(_user, partyroomnum);
			if (check)
			{
				// 유저 추가 성공
				User_Pack_Party_CharacterInfoToOther(_user, rdata, rdatasize);
				User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_ADD_USER, rdata, rdatasize);
				sendprotocol = SERVER_INGAME_PARTY_ROOM_ADD_USER;
				_user->Quepack(sendprotocol, rdata, rdatasize);
			}
			else
			{
				// 유저 추가 실패(초대 받는사람한테 실패라고 보낸다)
				User_Pack_Party_InviteResultToOther(_user, rdata, rdatasize);

				sendprotocol = SERVER_INGAME_PARTY_ROOM_JOIN_RESULT;
				_user->Quepack(sendprotocol, buf, datasize);
			}
		}
		else // 초대 거절
		{
			// 파티 초대 거절 패킹하고 보내준다.
			User_Pack_Party_Result(_user, false, rdata, rdatasize);
			User_Send_Party_InviteToOther(_user, SERVER_INGAME_PARTY_ROOM_INVITE_RESULT, rdata, rdatasize, charactercode);

			// 유저가 혼자라면 삭제한다.
			if (User_PartyRoom_Alone(partyroomnum))
			{
				// 만들어둔 파티방 삭제
				User_Remove_PartyRoom(charactercode);
			}
		}
		result = RT_INGAME_PARTY_INVITE_RESULT;
		break;
	case CLIENT_INGAME_PARTY_USER_KICK:		// 파티 강퇴 요청
		User_Unpack_UserCode(_user, buf, charactercode);
		if (_user->isPartyLeader())
		{
			// 강퇴 성공
			if (User_PartyRoom_User_Kick(_user, charactercode))
			{
				// 강퇴당한 유저한테 알려주기
				User_Pack_Party_Protocol(_user, buf, datasize);
				User_Send_Party_InviteToOther(_user, SERVER_INGAME_PARTY_USER_KICK, buf, datasize, charactercode);

				// 파티원들 한테는 나갔다라는 코드만 알려주기
				User_Pack_Party_KickInfo(_user, rdata, rdatasize, charactercode);
				User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_KICK_INFO, rdata, rdatasize);

				// 파티리더(현재유저)에게는 파티강퇴 실패여부를 알려준다. 그리고 강퇴당한 친구 코드를 넣어준다.
				User_Pack_Party_Result_Code(_user, true, charactercode, buf, datasize);
				sendprotocol = SERVER_INGAME_PARTY_USER_KICK_RESULT;
				_user->Quepack(sendprotocol, buf, datasize);
			}
			else
			{
				User_Pack_Party_Result_Code(_user, false, charactercode, buf, datasize);
				sendprotocol = SERVER_INGAME_PARTY_USER_KICK_RESULT;
				_user->Quepack(sendprotocol, buf, datasize);
			}

		}
		result = RT_INGAME_PARTY_USER_KICK_RESULT;
		break;
	case CLIENT_INGAME_PARTY_USER_LEAVE:	// 파티 탈퇴 요청
		if (_user->isPartyLeader())
		{
			// 파티원들한테 파티방 터트렸다는 패킷을보냄
			User_Pack_Party_Protocol(_user, rdata, rdatasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, rdata, rdatasize);

			// 파티없앤다
			check = User_Remove_PartyRoom(_user);

			// 현재유저(파티리더였던유저)에게 파티 탈퇴결과를 보낸다
			User_Pack_Party_Result(_user, check, buf, datasize);
			sendprotocol = SERVER_INGAME_PARTY_USER_LEAVE_RESULT;
			_user->Quepack(sendprotocol, buf, datasize);
		}
		else
		{
			// 파티원들한테 파티원 한명 탈퇴했다고 알려준다.
			User_Pack_UserCode(_user, rdata, rdatasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_INFO, rdata, rdatasize);

			// 파티에서 한명 지운다.
			check = User_PartRoom_User_Leave(_user);

			// 현재유저(파티원이였던유저)에게 파티 탈퇴결과를 보낸다
			User_Pack_Party_Result(_user, check, buf, datasize);
			sendprotocol = SERVER_INGAME_PARTY_USER_LEAVE_RESULT;
			_user->Quepack(sendprotocol, buf, datasize);

		}
		result = RT_INGAME_PARTY_USER_LEAVE_RESULT;
		break;
	case CLIENT_INGAME_PARTY_LEADER_DELEGATE:		// 파티장 위임
		User_Unpack_UserCode(_user, buf, charactercode);

		// 파티장 위임
		if (User_PartyRoom_Leader_Delegate(_user, charactercode))
		{
			// 파티장 위임 성공시 파티리더 바뀐정보를 파티원들한테 보낸다
			User_Pack_PartyRoom_Leader_Delegate(_user, rdata, rdatasize, charactercode);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_LEADER_DELEGATE, rdata, rdatasize);
			// 현재 유저에게 파티장 위임 결과를 보낸다.(코드+성공실패)
			User_Pack_Party_Result_Code(_user, true, charactercode, buf, datasize);
			_user->Quepack(SERVER_INGAME_PARTY_LEADER_DELEGATE_RESULT, buf, datasize);
		}
		else
		{
			// 현재 유저에게 파티장 위임 결과를 보낸다.(코드+성공실패)
			User_Pack_Party_Result_Code(_user, false, charactercode, buf, datasize);
			_user->Quepack(SERVER_INGAME_PARTY_LEADER_DELEGATE_RESULT, buf, datasize);
		}
		result = RT_INGAME_PARTY_LEADER_DELEGATE_RESULT;
		break;
	case CLIENT_INGAME_DUNGEON_ENTER:		// 던전 입장 요청
		User_EnterInDun_Channel(_user);
		User_Send_Party_Eneter_Dungeon(_user, SERVER_INGAME_OTHERPLAYER_LEAVE);
	
		User_Pack_Party_Dungeon_SpawnData(_user, buf, datasize);
		User_Send_Party_ToOther(_user, SERVER_INGAME_DUNGEON_ENTER_RESULT, buf,datasize );
		_user->Quepack(SERVER_INGAME_DUNGEON_ENTER_RESULT, buf, datasize);

		result = RT_INGAME_DUNGEON_ENTER_RESULT;
		break;
	case CLIENT_INGAME_DUNGEON_LEAVE:	// 던전 퇴장 요청

		User_LeaveInDun_Channel(_user);

		result = RT_INGAME_DUNGEON_LEAVE_RESULT;
		break;
	case CLIENT_INGAME_DUNGEON_STAGE_IN:	// 스테이지 입장 프로토콜
		// 스테이지 입장시 캐릭터 좌표 전송
		User_Pack_Party_Dungeon_Stage_SpawnData(_user, buf, datasize);
		User_Send_Party_ToOther(_user, SERVER_INGAME_DUNGEON_STAGE_IN_REULST, buf, datasize);
		_user->Quepack(SERVER_INGAME_DUNGEON_STAGE_IN_REULST, buf, datasize);

		// 스테이지 입장시 몬스터 정보 전송

		User_Send_Party_ToOther(_user, SERVER_INGAME_MONSTER_MOVE_RESULT, buf, datasize);
		_user->Quepack(SERVER_INGAME_MONSTER_MOVE_RESULT, buf, datasize);

		result = RT_INGAME_DUNGEON_STAGE_IN_RESULT;
		break;
	default:
		break;
	}

	return result;
}

// 인게임 상태에서 강종했을때 뒷처리
bool InGameManager::User_InGame_Compulsion_Exit(User * _user)
{
	char buf[BUFSIZE];
	char* ptr = buf;
	int datasize = 0;
	bool check = false;

	// 메세지
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));


	// 파티에 속해있고 던전에 들어와있으면
	if (_user->isParty() == true && _user->isDungeon() == true)
	{
		if (_user->isPartyLeader())
		{
			// 파티원들한테 파티방 터트렸다는 패킷을보냄
			User_Pack_Party_Protocol(_user, buf, datasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, buf, datasize);
			// 파티없앤다
			User_Remove_PartyRoom(_user);
		}
		else
		{
			// 나가는 유저의 정보를 패킹한다
			User_Pack_UserCode(_user, buf, datasize);
			// 파티원, 채널에 접속한 유저들한테 send한다.
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_INFO, buf, datasize);
			User_PartRoom_User_Leave(_user);
		}

		sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있고 던전에 들어와있는 상태에 종료");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Leave_Channel(_user);

		return true;
	} // 파티에 속해있고 던전에 들어와있지않고 채널에 들어와있으면
	else if (_user->isParty() == true && _user->isChannel() == true && _user->isDungeon() == false)
	{
		if (_user->isPartyLeader())
		{
			// 파티원들한테 파티방 터트렸다는 패킷을보냄
			User_Pack_Party_Protocol(_user, buf, datasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, buf, datasize);
			//User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, _user->GetChannelNum(), buf, datasize);

			// 파티없앤다
			User_Remove_PartyRoom(_user);
		}
		else
		{
			// 나가는 유저의 정보를 패킹한다
			User_Pack_UserCode(_user, buf, datasize);
			// 파티원, 채널에 접속한 유저들한테 send한다.
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_INFO, buf, datasize);
			User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, _user->GetChannelNum(), buf, datasize);
			User_PartRoom_User_Leave(_user);
		}

		User_Leave_Channel(_user);

		sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있고 던전에 들어와있지않고 채널에 들어와있는상태에 종료 ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	} // 파티에 속해있지않고 채널에 들어와있으면
	else if (_user->isParty() == false && _user->isChannel() == true)
	{
		User_Pack_UserCode(_user, buf, datasize);
		User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, _user->GetChannelNum(), buf, datasize);
		User_Leave_Channel(_user);

		sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있지않고 채널에 들어와있는상태에 종료");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	}

	return false;
}
