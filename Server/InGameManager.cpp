#include "InGameManager.h"

InGameManager* InGameManager::Instance = nullptr;

InGameManager::InGameManager()
{
	m_verification = nullptr;
	channelsystem = nullptr;
	m_partysystem = nullptr;
}

InGameManager::~InGameManager()
{
	delete m_verification;
	delete channelsystem;
	delete m_partysystem;
}

// 스레드 함수(몬스터 스폰)
DWORD __stdcall InGameManager::MonsterSpawnTimerProcess(LPVOID _user)
{
	User* user = (User*)_user;

	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	char* ptr = buf;

	UINT64 sendprotocol = 0;
	// 타이머
	MonsterTime monster_timer;
	int time = 0;
	int datasize = 0;

	int monstercode = 0;
	int monsternum = 0;
	int monster_typecount = 0;
	int monster_maxnum = 0;
	int spawnposcount = 0;
	int spawnpos_maxcount = 2;
	Vector3 pos[MONSTER_SPAWNPOS_MAXCOUNT];
	GameDataManager::GetInstance()->Dungeon_Monster_SpawnPos_Vector(pos);

	PartyRoom* partyroom = InGameManager::GetInstance()->GetPartyRoomSearch(user);

	if (partyroom == nullptr)
	{
		return 1;
	}

	monster_typecount = partyroom->GetMonsterTypes();

	for (int i = 0; i < monster_typecount; i++)
	{
		// 파티룸이 없어지면
		if (partyroom == nullptr)
		{
			return 1;
		}

		monstercode = partyroom->GetMonsterCode(i);
		monster_maxnum = partyroom->GetMonsterNum(monstercode);

		for (int d = 0; d < monster_maxnum; d++)
		{
			// 타이머 시작
			monster_timer.Start_Time();

			while (1)
			{
				// 타이머 확인
				if (monster_timer.End_Time() < 1)
				{
					continue;
				}

				// 초기화
				sendprotocol = 0;
				memset(buf, 0, sizeof(buf));
				ptr = buf;
				datasize = 0;

				// 파티룸이 없어지면
				if (partyroom == nullptr)
				{
					return 1;
				}

				if (partyroom->GetIsBossStage())
				{
					spawnposcount = BOSS_MONSTER_SPWANPOS_NUM;
				}
				else
				{
					spawnposcount = RandomNumberManager::GetInstance()->GetRandomNumber(NORMAL_MONSTER_SPAWNPOS_COUNT);
				}

				InGameManager::GetInstance()->User_Pack_Dungeon_Monster_SpawnInfo(user, buf, datasize, monstercode, d, pos[spawnposcount]);
				sendprotocol = user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_INFO);
				InGameManager::GetInstance()->User_Send_Party_ToOther(user, sendprotocol, buf, datasize);
				user->Quepack(sendprotocol, buf, datasize);

				break;
			} // while end
		} // for(d < monster_maxnum) end
	} // for ( i < monster_typecount) end

	monster_timer.Start_Time();


	return 1;
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
	m_verification = new CharacterVerification();
	channelsystem = new ChannelSystem();
	m_partysystem = new PartySystem();

	m_verification->Initialize(0, 0);
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
	UINT64 sendprotocol = 0;
	char data[BUFSIZE];
	memset(data, 0, sizeof(data));
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

	// 동기화
	CThreadSync cs;

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

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYERLIST_INFO);
	_user->Quepack(sendprotocol, data, size);
}

// 접속한 유저 정보 패킹
void InGameManager::User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize)
{
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
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "이동 데이터 :: 아이디: [%s] 위치: [%f] [%f] [%f]", _user->getID(), curPos.x, curPos.y,
	//	curPos.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 버퍼 클리어
	ptr = _buf;
	memset(_buf, 0, sizeof(_buf));

	// 이동검증
	//
	//lesult = m_verification->CharacterMoveVerificate(curPos, prePos, _user->GetCurCharacter()->GetCharacter_Speed());
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

	//// 메세지
	//char msg[BUFSIZE];
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "회전 데이터 :: 아이디: [%s] 회전: [%f] [%f] [%f]", _user->getID(), curRot.x, curRot.y,
	//	curRot.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
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

	//// 메세지
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "User_Pack_UserCode :: 유저의 코드 패킹 : [%s] ", curcharacter->GetCharacter_Code());
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 채널 정보 전송
void InGameManager::User_Pack_ChannelInfo(User * _user)
{
	UINT64 sendprotocol = 0;
	char data[BUFSIZE];
	char* ptr = data;
	int size = 0;
	int usercount = 0;

	char msg[BUFSIZE];

	CThreadSync cs;

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

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHANNEL, PROTOCOL_CHANNLE_INFO);
	_user->Quepack(sendprotocol, data, size);
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

	CThreadSync cs;

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

	// 동기화
	CThreadSync cs;

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
				character_hp = character_temp->GetCharacter_HP();
				character_mp = character_temp->GetCharacter_MP();

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
}

// 파티 초대했는데 방에 못들어갔을때
void InGameManager::User_Pack_Party_InviteResultToOther(User * _user, char * _data, int & _datasize, bool _result)
{
	int datasize = 0;
	char* ptr = _data;
	char msg[BUFSIZE];
	bool result = false;

	// 결과
	memcpy(ptr, &_result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// 메세지
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "User_Pack_Party_InviteResultToOther ::  ");
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

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

	// 동기화
	CThreadSync cs;

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

	// 동기화
	CThreadSync cs;

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
}

// 던전 스테이지 입장시 몬스터 정보.
void InGameManager::User_Pack_Dungeon_Monster_SpawnInfo(User * _user, char * _data, int & _datasize)
{
	char* ptr = _data;
	int size = 0;
	char msg[BUFSIZE];
	PartyRoom* partyroom_temp = nullptr;
	int monstercode = 0;
	int monsternum = 0;
	float time = MONSTERSPAWNTIME;
	Vector3 spawnpos[MONSTER_SPAWNPOS_MAXCOUNT];

	GameDataManager::GetInstance()->Dungeon_Monster_SpawnPos_Vector(spawnpos);

	// 동기화
	CThreadSync cs;

	// 파티방
	partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	int monster_typecount = partyroom_temp->GetMonsterTypes();

	// 몬스터 종류 숫자
	memcpy(ptr, &monster_typecount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 몬스터 스폰 시간
	memcpy(ptr, &time, sizeof(float));
	ptr += sizeof(float);
	size += sizeof(float);

	for (int i = 0; i < monster_typecount; i++)
	{
		// 몬스터 코드
		monstercode = partyroom_temp->GetMonsterCode(i);
		monsternum = FIRSTSTAGE_NORMALMONSTER_1;

		memcpy(ptr, &monstercode, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// 몬스터 숫자
		memcpy(ptr, &monsternum, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// 몬스터 등장 좌표
		memcpy(ptr, &spawnpos[2], sizeof(Vector3));
		ptr += sizeof(Vector3);
		size += sizeof(Vector3);

		// 메세지
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "몬스터 스폰 좌표 :: 몬스터 코드: [%d] 몬스터 숫자: [%d] 위치: [%f] [%f] [%f]", monstercode, monsternum, spawnpos[i].x, spawnpos[i].y,
			spawnpos[i].z);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	_datasize = size;
}

// 던전 스테이지 입장시 몬스터 정보.(몬스터코드,몬스터숫자,스폰될좌표)
void InGameManager::User_Pack_Dungeon_Monster_SpawnInfo(User * _user, char* _data, int& _datasize,  int _code, int _num, Vector3 _pos)
{
	char* ptr = _data;
	int size = 0;
	char msg[BUFSIZE];

	// 몬스터 코드
	memcpy(ptr, &_code, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 몬스터 숫자
	memcpy(ptr, &_num, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 몬스터 등장 좌표
	memcpy(ptr, &_pos, sizeof(Vector3));
	ptr += sizeof(Vector3);
	size += sizeof(Vector3);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "몬스터 스폰 좌표 :: 몬스터 코드: [%d] 몬스터 숫자: [%d] 위치: [%f] [%f] [%f]", _code, _num, _pos.x, _pos.y,
		_pos.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = size;
}

// 공격정보 패킹(유저코드,공격정보)
void InGameManager::User_Pack_AttackNum_Info(User * _user, char * _data, int & _datasize, int _attacknum)
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

	// 공격번호 - (애니메이션번호)
	memcpy(ptr, &_attacknum, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	//// 메세지
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "User_Pack_Attack_Info :: 유저의 코드 패킹 : [%s] 유저의 공격번호 : [%d] ", curcharacter->GetCharacter_Code(), _attacknum);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 공격정보 패킹(몬스터코드,몬스터번호,공격번호)
void InGameManager::User_Pack_Monster_Attack_Info(User * _user, char * _data, int & _datasize, int _code, int _num, int _attackcode)
{
	char* ptr = _data;
	int size = 0;

	char msg[BUFSIZE];

	// 몬스터 코드
	memcpy(ptr, &_code, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 몬스터 번호
	memcpy(ptr, &_num, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 공격 코드
	memcpy(ptr, &_attackcode, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	_datasize = size;

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "몬스터 공격 정보 전송. 코드 : [%d] 번호 : [%d] 공격번호: [%d]", _monstercode, _monsternum, _attacknum);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// 몬스터 피격결과. 다른유저한테 정보패킹(데미지,죽었다는의미)
void InGameManager::User_Pack_MonsterAttackToOher_Result(User * _user, char  *_data, int & _datasize, int _monstercode, int _monsternum, int _damage, bool _isdie)
{
	char* ptr = _data;
	int size = 0;

	char msg[BUFSIZE];

	// 몬스터 코드
	memcpy(ptr, &_monstercode, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 몬스터 번호
	memcpy(ptr, &_monsternum, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 입힌 데미지
	memcpy(ptr, &_damage, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 죽었는지. 죽으면 true 아니면 false
	memcpy(ptr, &_isdie, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);

	_datasize = size;

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "다른 유저에게 몬스터 정보 전송. 보내는 유저 코드 : [%s], 몬스터코드 : [%d], 번호 : [%d], 입힌 피해량 : [%d]",_user->GetCurCharacter()->GetCharacter_Code(), _monstercode, _monsternum, _damage);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// 유저 피격결과. 다른유저한테 정보패킹(캐릭터코드,데미지,죽었다는의미)
void InGameManager::User_Pack_User_UnderAttackToOher_Result(User * _user, char * _data, int & _datasize, int _damage, bool _isdie)
{
	char* ptr = _data;
	int size = 0;

	char msg[BUFSIZE];

	int len = strlen(_user->GetCurCharacter()->GetCharacter_Code());

	// 유저 코드 길이
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 유저 코드
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), len);
	ptr += len;
	size += len;

	// 입힌 데미지
	memcpy(ptr, &_damage, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// 죽었는지. 죽으면 true 아니면 false
	memcpy(ptr, &_isdie, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);

	_datasize = size;

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "다른 유저에게 몬스터 정보 전송. 코드 : [%d] 번호 : [%d] 입힌 피해량 : [%d]", _monstercode, _monsternum, _damage);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
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
		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [성공]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [실패]");
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

		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [성공]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: 파티 초대 결과 [실패]");
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

// 몬스터 이동정보 패킹(몬스터숫자,몬스터코드,몬스터번호,몬스터좌표)
void InGameManager::User_Pack_Monster_MoveInfo(User * _user, int _code, int _num, char*& _data, int& _datasize, UINT64& _sendprotocol)
{
	char* rptr = _data;
	char* ptr = rptr + sizeof(int);

	int size = sizeof(int);

	int code = 0;
	int monsternum = 0;
	UINT64 sendprotocol = _sendprotocol;

	PartyRoom* partyroom = nullptr;
	MonsterInfo* monsterinfo = nullptr;
	Vector3 pos;

	char msg[BUFSIZE];

	// 동기화
	CThreadSync cs;

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 몬스터 정보 검색 시작
	partyroom->StartSearchMonsterinfo();
	while (partyroom->SearchMonsterinfo(monsterinfo))
	{
		if (monsterinfo->GetMonster()->GetMonster_Code() == _code && monsterinfo->GetMonsterNum() == _num)
		{
			code = monsterinfo->GetMonster()->GetMonster_Code();
			monsternum = monsterinfo->GetMonsterNum();

			// 몬스터 코드
			memcpy(ptr, &code, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);

			// 몬스터 번호
			memcpy(ptr, &monsternum, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);

			// 몬스터 위치
			memcpy(ptr, &monsterinfo->GetMonster()->GetPosition(), sizeof(Vector3));
			ptr += sizeof(Vector3);
			size += sizeof(Vector3);

			// 패킷 길이
			memcpy(rptr, &size, sizeof(int));

			_datasize = size;
			_data = ptr;
			_sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_MOVE_RESULT);

			/*memset(msg, 0, sizeof(msg));
			sprintf(msg, "몬스터 정보 전송. 코드 : [%d] 번호 : [%d] x : [%f] y : [%f] z [%f]", code, monsternum, monsterinfo->GetMonster()->GetPosition().x, monsterinfo->GetMonster()->GetPosition().y, monsterinfo->GetMonster()->GetPosition().z);
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/

			break;
		}
	}
}

// 몬스터 피격결과 정보 패킹(성공했을때,몬스터코드,몬스터번호,입힌피해량,죽었는지)
void InGameManager::User_Pack_MonsterAttack_Result(User * _user, bool _result, int _monstercode, int _monsternum, int _damage, bool _isdie)
{
	UINT64 sendprotocol = 0;
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	int size = 0;
	char* ptr = buf;

	char msg[BUFSIZE];

	// 피격 성공했는지
	memcpy(ptr, &_result, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);

	if (_result)
	{
		// 몬스터 코드
		memcpy(ptr, &_monstercode, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// 몬스터 번호
		memcpy(ptr, &_monsternum, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// 입힌 데미지
		memcpy(ptr, &_damage, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// 죽었는지. 죽으면 true 아니면 false
		memcpy(ptr, &_isdie, sizeof(bool));
		ptr += sizeof(bool);
		size += sizeof(bool);

		memset(msg, 0, sizeof(msg));
		sprintf(msg, "몬스터 피격 정보. 유저 코드 : [%s], 코드 : [%d], 번호 : [%d], 입힌 피해량 : [%d]",_user->GetCurCharacter()->GetCharacter_Code(), _monstercode, _monsternum, _damage);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_ATTACK_RESULT);
	_user->Quepack(sendprotocol, buf, size);
}

// 유저 피격결과 정보패킹(성공실패,데미지,죽으면 false)
void InGameManager::User_Pack_Under_Attack_Result(User * _user, bool _result, int _damage, bool _state)
{
	UINT64 sendprotocol = 0;
	char buf[512];
	memset(buf, 0, sizeof(buf));
	int datasize = 0;
	char* ptr = buf;

	char msg[BUFSIZE];

	// 피격이 안됬으면
	if (_result == false)
	{
		memcpy(ptr, &_result, sizeof(bool));
		ptr += sizeof(bool);
		datasize += sizeof(bool);
	}
	// 공격받았지만 살아있음
	else if (_result == true)
	{
		memcpy(ptr, &_result, sizeof(bool));
		ptr += sizeof(bool);
		datasize += sizeof(bool);

		memcpy(ptr, &_damage, sizeof(int));
		ptr += sizeof(int);
		datasize += sizeof(int);

		memcpy(ptr, &_state, sizeof(bool));
		ptr += sizeof(bool);
		datasize += sizeof(bool);

		/*memset(msg, 0, sizeof(msg));
		sprintf(msg, "유저 피격 정보. 코드 : [%s] 입힌 피해량 : [%d]", _user->GetCurCharacter()->GetCharacter_Code(), _damage);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/
	}

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_MONSTER_ATTACK_RESULT);
	_user->Quepack(sendprotocol, buf, datasize);
}

// 몬스터 타겟 정보패킹(몬스터코드,몬스터번호,케릭터코드)
void InGameManager::User_Pack_Nearset_Character(User * _user, int _monstercode, int _monsternum, Character* _character, char*& _data, int& _datasize, UINT64& _sendprotocol)
{
	UINT64 sendprotocol = _sendprotocol;

	char* rptr = _data;
	char* ptr = rptr + sizeof(int);

	int size = sizeof(int);
	int datasize = _datasize;

	int len = 0;

	char msg[BUFSIZE];

	if (_character == nullptr)
	{
		return;
	}

	len = strlen(_character->GetCharacter_Code());

	// 몬스터 코드
	memcpy(ptr, &_monstercode, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	// 몬스터 번호
	memcpy(ptr, &_monsternum, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	// 코드 사이즈
	memcpy(ptr, &len, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	// 코드
	memcpy(ptr, _character->GetCharacter_Code(), len);
	size += len;
	ptr += len;

	// 코드 사이즈
	memcpy(rptr, &size, sizeof(int));

	_datasize += _datasize + size;
	_data = ptr;
	_sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_MONSTER_SET_TARGET);

	//// 메세지
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "User_Pack_Nearset_Character :: 가장 가까운 유저는 [캐릭터 코드 : %s] ", _character->GetCharacter_Code());
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
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

// 몬스터 이동 정보 언팩(몬스터코드,몬스터번호,좌표)
void InGameManager::User_Unpack_Monster_Move(User * _user, char * _buf, int& _code, int& _num)
{
	Vector3 prePos;
	Vector3 curPos;
	PartyRoom* partyroom = nullptr;
	MonsterInfo* monsterinfo = nullptr;
	float dirx, diry;
	bool lesult;
	int datasize = 0;
	int len = 0;
	int monster_code = 0;
	int monster_num = 0;
	char* ptr = _buf;
	char msg[BUFSIZE];

	// 몬스터 코드 복사
	memcpy(&monster_code, ptr, sizeof(int));
	ptr += sizeof(int);

	// 몬스터 번호 복사
	memcpy(&monster_num, ptr, sizeof(int));
	ptr += sizeof(int);

	// 위치 복사
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (partyroom->GetMonsterinfo(monster_code, monster_num, monsterinfo) == false)
	{
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "몬스터 이동 :: 좌표 받기 실패");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return;
	}

	// 이동검증


	// 몬스터 정보 저장
	partyroom->SetMonsterinfo(monster_code, monster_num, curPos);

	_code = monster_code;
	_num = monster_num;
	// 메세지
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "몬스터 이동 :: 몬스터 코드: [%d]  몬스터 번호: [%d] 위치: [%f] [%f] [%f]", monster_code, monster_num, curPos.x, curPos.y, curPos.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// 몬스터 이동 정보 언팩(몬스터코드,몬스터번호,좌표,방향)
void InGameManager::User_Unpack_BossMonster_Move(User * _user, char * _buf, int & _code, int & _num)
{
	Vector3 prePos;
	Vector3 curPos;
	Vector3 tempDir;
	Vector3 dir;
	PartyRoom* partyroom = nullptr;
	MonsterInfo* monsterinfo = nullptr;
	float dirx, diry;
	bool lesult;
	int datasize = 0;
	int len = 0;
	int monster_code = 0;
	int monster_num = 0;
	char* ptr = _buf;
	char msg[BUFSIZE];

	// 몬스터 코드 복사
	memcpy(&monster_code, ptr, sizeof(int));
	ptr += sizeof(int);

	// 몬스터 번호 복사
	memcpy(&monster_num, ptr, sizeof(int));
	ptr += sizeof(int);

	// 위치 복사
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	// 방향 복사
	memcpy(&tempDir, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	dir = curPos.direction(tempDir);

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (partyroom->GetMonsterinfo(monster_code, monster_num, monsterinfo) == false)
	{
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "몬스터 이동 :: 좌표 받기 실패");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return;
	}

	// 이동검증


	// 몬스터 정보 저장
	partyroom->SetMonsterinfo(monster_code, monster_num, curPos, dir);

	_code = monster_code;
	_num = monster_num;
	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "몬스터 이동 :: 몬스터 코드: [%d]  몬스터 번호: [%d] 위치: [%f] [%f] [%f]", monster_code, monster_num, curPos.x, curPos.y, curPos.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// 유저가 특정 몬스터를 공격했다는 패킷
void InGameManager::User_Unpack_User_Successfully_Attack_The_Monster(User * _user, char * _buf, int & _monstercode, int & _monsternum, int& _attacknum, Vector3& _dir)
{
	int monster_code = 0;
	int monster_num = 0;
	int attacknum = 0;
	char* ptr = _buf;

	// 몬스터 코드 복사
	memcpy(&monster_code, ptr, sizeof(int));
	ptr += sizeof(int);

	// 몬스터 번호 복사
	memcpy(&monster_num, ptr, sizeof(int));
	ptr += sizeof(int);

	// 공격 번호
	memcpy(&attacknum, ptr, sizeof(int));
	ptr += sizeof(int);

	// 방향 벡터 x
	memcpy(&_dir.x, ptr, sizeof(float));
	ptr += sizeof(float);

	// 방향 벡터 y
	memcpy(&_dir.y, ptr, sizeof(float));
	ptr += sizeof(float);

	// 방향 벡터 z
	memcpy(&_dir.z, ptr, sizeof(float));
	ptr += sizeof(float);

	_monstercode = monster_code;
	_monsternum = monster_num;
	_attacknum = attacknum;
}

// 유저가 무슨공격했는지 언팩(공격정보)
void InGameManager::User_Unpack_Attack_Info(User * _user, char * _buf, int & _attacknum)
{
	int attacknum = 0;

	memcpy(&attacknum, _buf, sizeof(int));

	_attacknum = attacknum;
}

// 몬스터가가 무슨공격했는지 언팩(공격정보)
void InGameManager::User_Unpack_Monster_Successfully_Attack_The_User(User * _user, char * _buf, int & _monstercode, int & _monsternum, int & _attacknum, Vector3& _dir)
{
	int monster_code = 0;
	int monster_num = 0;
	int attacknum = 0;
	char* ptr = _buf;

	// 몬스터 코드 복사
	memcpy(&monster_code, ptr, sizeof(int));
	ptr += sizeof(int);

	// 몬스터 번호 복사
	memcpy(&monster_num, ptr, sizeof(int));
	ptr += sizeof(int);

	// 공격 번호
	memcpy(&attacknum, ptr, sizeof(int));
	ptr += sizeof(int);

	// 공격사거리 벡터 x
	memcpy(&_dir.x, ptr, sizeof(float));
	ptr += sizeof(float);

	// 공격사거리 벡터 y
	memcpy(&_dir.y, ptr, sizeof(float));
	ptr += sizeof(float);

	// 공격사거리 벡터 z
	memcpy(&_dir.z, ptr, sizeof(float));
	ptr += sizeof(float);

	_monstercode = monster_code;
	_monsternum = monster_num;
	_attacknum = attacknum;
}

// 이동이나 회전 정보 Vecotr3. 
void InGameManager::User_Pack_MoveInfoToOther(User* _user, char * _data, int & _datasize)
{
	int datasize = 0;
	int len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	char* ptr = _data;
	char msg[BUFSIZE];
	int animationnum = 0;
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

	//// 애니메이션 번호
	//memcpy(ptr, &animationnum, sizeof(int));
	//datasize += sizeof(int);
	//ptr += sizeof(int);

	//// 메세지
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "User_Pack_MoveInfoToOther :: 코드 : [%s] 위치: [%f] [%f] [%f]", curcharacter->GetCharacter_Code(), curcharacter->GetPosition().x, curcharacter->GetPosition().y,
	//	curcharacter->GetPosition().z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 현재 캐릭터 DB에 저장
void InGameManager::User_CurCharacter_Save(User * _user)
{
	CThreadSync cs;
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

// 유저 공격시 피격 판정
bool InGameManager::User_Attack_Result(User * _user, char* _buf)
{
	char buf[BUFSIZE];
	char rdata[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	memset(rdata, 0, sizeof(rdata));
	int datasize = 0;
	int rdatasize = 0;
	char* ptr = buf;

	// 로그용
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	UINT64 sendprotocol = 0;
	UINT64 othersendprotocol = 0;

	Vector3 dir;
	PartyRoom* partyroom = nullptr;

	bool flag = true;
	int monstercode = 0;
	int monsternum = 0;
	int attacknum = 0;

	// 몬스터 정보를 클라한테 받는다.
	User_Unpack_User_Successfully_Attack_The_Monster(_user, _buf, monstercode, monsternum, attacknum, dir);

	// 유저 캐릭터 및 스킬정보 셋팅
	Character* user_character = _user->GetCurCharacter();
	AttackInfo user_attackinfo = _user->GetCurCharacter()->GetAttackInfo(attacknum);

	// 몬스터 정보 셋팅.
	MonsterInfo* monster = nullptr;
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티방이 없거나 파티중이 아니면
	if (partyroom == nullptr && _user->isParty() == false)
	{
		return false;
	}

	// 몬스터가 없거나 몬스터가 이미 죽었으면
	if (partyroom->GetMonsterinfo(monstercode, monsternum, monster) == false)
	{
		//// 메세지
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "공격 피격 판정 : 몬스터가 존재하지않습니다. [몬스터코드 : %d] [몬스터 번호 : %d] ", monstercode, monsternum);
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Pack_MonsterAttack_Result(_user, false, monstercode, monsternum, 0, false);
		return false;
	}

	if (monster->GetMonsterActivate() == false)
	{
		//// 메세지
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "공격 피격 판정 : 몬스터가 이미 죽었습니다. [몬스터코드 : %d] [몬스터 번호 : %d] ", monstercode, monsternum);
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Pack_MonsterAttack_Result(_user, false, monstercode, monsternum, 0, false);
		return false;
	}

	// -- 피격판정 --
	// 몬스터와 거리가 가까우면
	if (m_verification->AttackVerificate(user_character->GetPosition(), dir, user_attackinfo.attack_range, user_attackinfo.attack_angle, monster->GetMonster()->GetPosition(), monster->GetMonster()->GetMonsterRange()) == false)
	{
		// 메세지
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "공격 피격 판정 : 몬스터가 피격되지않았습니다. [몬스터코드 : %d] [몬스터 번호 : %d] ", monstercode, monsternum);
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Pack_MonsterAttack_Result(_user, false, monstercode, monsternum, 0, false);
		return false;
	}

	// -- 데미지 처리 -- 
	// 피격성공 데미지를 주고 체력이 0이되면 활성화를 풀고 죽었다고 알려주자
	bool islive = partyroom->Monster_HP_Down(monstercode, monsternum, 10);

	// 유저에게 줄 정보 pack
	User_Pack_MonsterAttack_Result(_user, true, monstercode, monsternum, 10, islive);

	// 다른 유저에게 줄 정보 pack
	User_Pack_MonsterAttackToOher_Result(_user, rdata, rdatasize, monstercode, monsternum, 10, islive);
	othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_ATTACK_SUCCESS);

	if (_user->isDungeon())
	{
		User_Send_ToOther(_user, othersendprotocol, PARTY_TO_OTHER, rdata, rdatasize, 0, nullptr);
	}
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
	CThreadSync cs;
	return m_partysystem->Party_Remove(_user);
}

// 파티방 삭제 코드
bool InGameManager::User_Remove_PartyRoom(char * _code)
{
	User* user = nullptr;

	user = UserManager::GetInstance()->getUser(_code);

	CThreadSync cs;
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
	CThreadSync cs;
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
	CThreadSync cs;
	PartyRoom* partyroom = nullptr;
	// 파티 검색
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티 채널추가
	channelsystem->DungeonEnter(partyroom);
}

// 던전 채널 나가기
void InGameManager::User_LeaveInDun_Channel(User * _user)
{
	UINT64 sendprotocol = 0;
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

	// 동기화
	CThreadSync cs;

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (partyroom == nullptr || channelsystem->DungeonLeave(_user->GetPartyNum()) == false)
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
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_LEAVE_RESULT);
				user->Quepack(sendprotocol, buf, datasize);

				sendprotocol = 0;
				User_Pack_PlayerPosData(_user, rdata, rdatasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_CONNECT);
				User_Send_In_The_Channel(_user, sendprotocol, rdata, rdatasize);
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
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_LEAVE_RESULT);
			user->Quepack(sendprotocol, buf, datasize);

			sendprotocol = 0;

			User_Pack_PlayerPosData(_user, rdata, rdatasize);
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_CONNECT);
			User_Send_In_The_Channel(_user, sendprotocol, rdata, rdatasize);
		}
	}
}

PartyRoom * InGameManager::GetPartyRoomSearch(User * _user)
{
	return m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());
}


// 스테이지 상승 및 몬스터정보 셋팅
void InGameManager::User_Dungeon_Stage_Rise(User * _user)
{
	CThreadSync cs;
	PartyRoom* partyroom = nullptr;
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());
	if (partyroom == nullptr)
	{
		return;
	}
	partyroom->RiseStage();
	partyroom->SetDungeonMonsterinfo();
}

// send함수 통합(enum문으로 타입을 정해서 만들자)
void InGameManager::User_Send_ToOther(User * _user, UINT64 _p, SEND_TYPE _type, char * _data, int _datasize, int _channelnum, char* _code)
{
	switch (_type)
	{
	case CHANNEL: // 채널유저
		User_Send_In_The_Channel(_user, _p, _data, _datasize);
		break;
	case PARTY:
		User_Send_Party(_user, _p, _data, _datasize);
		break;
	case PARTY_TO_OTHER: // 파티원
		User_Send_Party_ToOther(_user, _p, _data, _datasize);
		break;
	case SPECIFIC_CHANNEL: // 특정 채널
		User_Send_In_a_Particular_Channel(_user, _p, _channelnum, _data, _datasize);
		break;
	case SPECIFIC_USER: // 특정 유저
		User_Send_Party_InviteToOther(_user, _p, _data, _datasize, _code);
		break;
	case CHANNEL_EXCLUDE_PARTY: // 채널(파티원제외)
		User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(_user, _p, _user->GetChannelNum(), _data, _datasize);
		break;
	case  ENTER_DUNGEON:			// 던전에 입장시(각 유저가 해당했던 채널에 캐릭터코드를 send)
		User_Send_Party_Enter_Dungeon(_user);
		break;
	case  LEAVE_DUNGEON:			// 던전에서 나갈시(각 유저가 해당한 채널에 캐릭터코드를 send)
		User_Send_Party_Leave_Dungeon(_user);
		break;
	default:
		break;
	}

}

// 다른 유저 이동정보 전송(채널에 접속해있는 유저들한테 전송)
void InGameManager::User_Send_In_The_Channel(User* _user, UINT64 _p, char * _data, int & _datasize)
{
	User* user = nullptr;

	// 동기화
	CThreadSync cs;

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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_In_The_Channel :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
}

// 다른 유저 인게임에서 떠난 정보 전송
void InGameManager::User_Send_LeaveInfoToOther(User * _user, UINT64 _p, char * _data, int & _datasize)
{
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	char* ptr = buf;
	int datasize = 0;
	UINT64 sendprotocol = 0;
	User* user = nullptr;
	
	// 유저 코드 pack
	User_Pack_UserCode(_user, buf, datasize);

	// 동기화
	CThreadSync cs;
	// 파티원 먼저 정보보낸다
	// 파티에 속해있으며 파티방장이 아니면 그냥 파티원들한테 보낸다
	if (_user->isParty() && _user->isPartyLeader() == false)
	{
		sendprotocol = _user->BitPackProtocol(sendprotocol,PROTOCOL_INGAME,PROTOCOL_INGAME_PARTY,PROTOCOL_PARTY_USER_LEAVE_INFO);
		User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, datasize, 0, nullptr);
	}// 파티에 속해있으며 파티방장이면 파티방 터트린다.
	else if (_user->isParty() && _user->isPartyLeader())
	{
		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_REMOVE_RESULT);
		User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, datasize, 0, nullptr);

		// 파티없앤다
		User_Remove_PartyRoom(_user);
	}

	// 마지막으로 채널에 보낸다
	// 채널에 속해있으면 채널에 속한 유저한테 보낸다
	if (_user->isChannel())
	{
		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
		User_Send_ToOther(_user, sendprotocol, CHANNEL, buf, datasize, 0, nullptr);
	}


}

// 특정 채널에 속해있는 유저들한테 전송(채널이동할때 사용)
void InGameManager::User_Send_In_a_Particular_Channel(User * _user, UINT64 _p, int _channelnum, char * _data, int & _datasize)
{
	User* user = nullptr;

	// 동기화
	CThreadSync cs;

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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_In_a_Particular_Channel :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
}

// 다른 유저 인게임에서 떠난 정보 채널에 전송(파티원 제외)
void InGameManager::User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(User * _user, UINT64 _p, int _channelnum, char * _data, int & _datasize)
{
	User* user = nullptr;

	// 동기화
	CThreadSync cs;

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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
}

// 특정 유저(code) 파티 초대 전송
void InGameManager::User_Send_Party_InviteToOther(User* _user, UINT64 _p, char* _data, int& _datasize, char* _code)
{
	User* user = nullptr;

	// 동기화
	CThreadSync cs;

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
		//char msg[BUFSIZE];
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "User_Send_Party_InviteToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
		//	_p, _datasize, user->GetSendQueueSize());
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
}

// 파티원에게 전송
void InGameManager::User_Send_Party_ToOther(User * _user, UINT64 _p, char * _data, int & _datasize)
{
	User* user = nullptr;
	PartyRoom* partyroom = nullptr;

	// 동기화
	CThreadSync cs;
	
	if (_user->isParty() == false)
	{
		return;
	}

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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_Party_ToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
}

// 파티원에게 전송(해당 유저 포함)
void InGameManager::User_Send_Party(User * _user, UINT64 _p, char * _data, int & _datasize)
{
	User* user = nullptr;
	PartyRoom* partyroom = nullptr;

	// 동기화
	CThreadSync cs;

	if (_user->isParty() == false)
	{
		return;
	}

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	partyroom->StartSearchPartyRoom();

	_user->Quepack(_p, _data, _datasize);

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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_Party_ToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
}

// 던전에 들어갔을대 채널에 속해있는 유저들한테 전송
void InGameManager::User_Send_Party_Enter_Dungeon(User * _user)
{
	PartyRoom* partyroom = nullptr;
	User* user = nullptr;
	char buf[BUFSIZE];
	int datasize = 0;
	memset(buf, 0, sizeof(buf));

	// 프로토콜 셋팅
	UINT64 sendprotocol = 0;
	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);

	CThreadSync cs;
	// 유저가 속해있는 파티방을 검색해서 넣는다.
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티방의 각 유저들 채널에 속해있는 유저들한테 나간다고 알려준다.
	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		// 유저 코드 패킹
		User_Pack_UserCode(user, buf, datasize);

		// 파티 유저가 속한 채널에 send
		User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(user, sendprotocol, user->GetChannelNum(), buf, datasize);
	}
}

// 던전에 나갔을때 채널에 속해있는 유저들한테 전송
void InGameManager::User_Send_Party_Leave_Dungeon(User * _user)
{
	PartyRoom* partyroom = nullptr;
	User* user = nullptr;
	char buf[BUFSIZE];
	int datasize = 0;
	memset(buf, 0, sizeof(buf));

	// 프로토콜 셋팅
	UINT64 sendprotocol = 0;
	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_CONNECT);

	CThreadSync cs;
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
		User_Send_In_The_Channel(user, sendprotocol, buf, datasize);
	}
}

// 몬스터 공격시 피격 판정
bool InGameManager::User_Under_Attack(User * _user, char* _buf)
{
	char buf[BUFSIZE];
	char rdata[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	memset(rdata, 0, sizeof(rdata));
	int datasize = 0;
	int rdatasize = 0;
	char* ptr = buf;

	// 로그용
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	UINT64 sendprotocol = 0;
	UINT64 othersendprotocol = 0;

	Vector3 dir;
	PartyRoom* partyroom = nullptr;

	bool flag = true;

	int monstercode = 0;
	int monsternum = 0;
	int attacknum = 0;
	// 몬스터 공격정보
	User_Unpack_Monster_Successfully_Attack_The_User(_user, _buf, monstercode, monsternum, attacknum, dir);

	// 유저 캐릭터 셋팅
	Character* user_character = _user->GetCurCharacter();

	// 몬스터 정보 셋팅.
	MonsterInfo* monster = nullptr;
	MonsterAttackInfo monster_attackinfo;
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티방이 없으면 탈출
	if (partyroom == nullptr)
	{
		return false;
	}
	if (_user->isParty() == false)
	{
		return false;
	}

	// 유저가 죽어있다면
	if (_user->GetCurCharacter_IsLive() == false)
	{
		User_Pack_Under_Attack_Result(_user, false, 0, flag);

		return false;
	}

	// 몬스터가 없으면
	if (partyroom->GetMonsterinfo(monstercode, monsternum, monster) == false)
	{
		//// 메세지
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "몬스터 공격 피격 판정 : 몬스터가 존재하지않습니다. [몬스터코드 : %d] [몬스터 번호 : %d] ", monstercode, monsternum);
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Pack_Under_Attack_Result(_user, false, 0, flag);

		return false;
	}

	// 몬스터가 이미 죽었으면
	if (monster->GetMonsterActivate() == false)
	{
		// 메세지
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "공격 피격 판정 : 몬스터가 이미 죽었습니다. [몬스터코드 : %d] [몬스터 번호 : %d] ", monstercode, monsternum);
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Pack_Under_Attack_Result(_user, false, 0, flag);

		return false;
	}

	// 몬스터 공격정보
	if (monster->GetMonster()->GetAttackInfo(attacknum, monster_attackinfo) == false)
	{
		return false;
	}

	// -- 피격판정 -- 몬스터 공격타입에 따른 피격 판정 계산
	switch (monster_attackinfo.attack_type)
	{
	case ATTACK_TYPE_MELEE: // 근접공격
		// 유저와 거리가 가까우면
		if (m_verification->AttackVerificate(monster->GetMonster()->GetPosition(), dir, monster_attackinfo.attack_range, monster_attackinfo.attack_angle, user_character->GetPosition(), user_character->GetCharacterRange()) == false)
		{
			// 메세지
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "공격 피격 판정 : 유저가 피격되지않았습니다. [몬스터코드 : %d] [몬스터 번호 : %d] ", monstercode, monsternum);
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);

			User_Pack_Under_Attack_Result(_user, false, 0, flag);

			return false;
		}
		break;
	case ATTACK_TYPE_LONG_RANGE: // 원거리 공격
		break;
	case ATTACK_TYPE_EXPLOSION: // 광역 공격(범위있음)
		// 유저와 거리가 가까우면
		if (m_verification->AttackVerificate(monster->GetMonster()->GetPosition(), monster_attackinfo.attack_range, user_character->GetPosition(), user_character->GetCharacterRange()) == false)
		{
			// 메세지
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "공격 피격 판정 : 유저가 피격되지않았습니다. [몬스터코드 : %d] [몬스터 번호 : %d] ", monstercode, monsternum);
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);

			User_Pack_Under_Attack_Result(_user, false, 0, flag);

			return false;
		}
		break;
	case ATTACK_TYPE_FIELD: // 광역 공격(필드전체)
		break;
	default: // 공격정보가 없으면
		return false;
	}

	// -- 데미지 처리 -- 
	// 피격성공 데미지를 주고 체력이 0이되면 ....
	flag = _user->CurCharacter_HP_Down(monster->GetMonster()->GetMonster_AttackPoint());

	User_Pack_Under_Attack_Result(_user, true, 10, flag);

	// 다른 유저 패킷처리 
	User_Pack_User_UnderAttackToOher_Result(_user, rdata, rdatasize, monster->GetMonster()->GetMonster_AttackPoint(), flag);

	othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_MONSTER_OTHER_ATTACK_RESULT);

	if (_user->isDungeon())
	{
		User_Send_Party_ToOther(_user, othersendprotocol, rdata, rdatasize);
	}
	else
	{
		//User_Send_In_The_Channel(_user, othersendprotocol, rdata, rdatasize);
	}

	return true;
}

// 몬스터 시간 초기화
void InGameManager::User_PartyRoom_Monster_Time_ReSet(User * _user, int _code, int _num)
{
	PartyRoom* partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	partyroom->Start_MonsterTimer(_code, _num);
}

// 해당 유저의 파티의 몬스터 시간이 정해둔 시간을 지났는가.
bool InGameManager::User_PartyRoom_Monster_TimeOver_Check(User* _user, int _code, int _num)
{
	PartyRoom* partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (partyroom == nullptr)
	{
		return false;
	}

	return partyroom->End_MonsterTimer(_code, _num);
}

// 이몬스터는 보스몬스터인가
bool InGameManager::Is_BossMonster(User * _user, int _code)
{
	PartyRoom* partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (partyroom == nullptr)
	{
		return false;
	}

	return partyroom->Is_BossMonster(_code);
}

// 몬스터 정보 가져오기
bool InGameManager::GetMonsterInfo(User* _user, int _code, int _num, MonsterInfo*& _info)
{
	PartyRoom* partyroom = nullptr;

	// 몬스터 정보 셋팅.
	MonsterInfo* monster = nullptr;
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// 파티방이 없으면 탈출
	if (partyroom == nullptr)
	{
		return false;
	}
	if (_user->isParty() == false)
	{
		return false;
	}

	// 몬스터가 없으면
	if (partyroom->GetMonsterinfo(_code, _num, monster) == false)
	{
		return false;
	}
	// 몬스터가 이미 죽었으면
	if (monster->GetMonsterActivate() == false)
	{
		return false;
	}
	_info = monster;

	return true;
}

// 가장 가까운 유저는 누구인가
bool InGameManager::GetNearestChracter(User * _user, int _monstercode, int _monsternum, float& _length, Character *& _character)
{
	int code = 0;
	int num = 0;
	float length = 0;
	PartyRoom* partyroom = nullptr;
	MonsterInfo* monsterinfo = nullptr;
	Character* character = nullptr;
	Vector3 pos;

	char msg[BUFSIZE];

	// 동기화
	CThreadSync cs;

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (GetMonsterInfo(_user, _monstercode, _monsternum, monsterinfo) == false)
	{
		return false;
	}

	if (monsterinfo->Is_BossMonster())
	{
		// 공격 가능한가(컨트롤러의 보스몬스터가 공격중인지 확인한다)
		if (partyroom->Is_BossMonster_Attackable() == false)
		{
			return false;
		}
	}

	// 제일 가까운 유저가 누구이고 거리가 얼마인가.
	length = m_verification->Is_Nearest_Chracter(partyroom, monsterinfo, character);

	_character = character;
	_length = length;

	return true;
}

// 몬스터 이동 정보 및 공격 판단
bool InGameManager::Monster_Req_Move(User * _user, char * _buf)
{
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));

	char rdata[BUFSIZE];
	memset(rdata, 0, sizeof(rdata));
	char* rptr = rdata;

	int datasize = 0;
	int rdatasize = 0;

	// 로그용
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	UINT64 sendprotocol = 0;
	UINT64 othersendprotocol = 0;

	int monstercode = 0;
	int monsternum = 0;
	int attackcode = 0;
	float length = 0;
	Vector3 dir;
	PartyRoom* partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());
	Character* character = nullptr;
	MonsterInfo* monster = nullptr;

	User_Unpack_Monster_Move(_user, buf, monstercode, monsternum);

	if (GetMonsterInfo(_user, monstercode, monsternum, monster))
	{
		if (monster->GetMonsterActivate())
		{
			// 시간이 지났으면 새로 정보를 보낸다
			if (User_PartyRoom_Monster_TimeOver_Check(_user, monstercode, monsternum))
			{
				User_Pack_Monster_MoveInfo(_user, monstercode, monsternum, rptr, rdatasize, sendprotocol);
				
				// 시간 초기화
				User_PartyRoom_Monster_Time_ReSet(_user, monstercode, monsternum);

				// 새로운 타겟을 정한다
				// 제일 가까운 유저가 누구이고 거리가 얼마인가.
				if (GetNearestChracter(_user, monster->GetMonster()->GetMonster_Code(), monster->GetMonsterNum(), length, character))
				{
					User_Pack_Nearset_Character(_user, monster->GetMonster()->GetMonster_Code(), monster->GetMonsterNum(), character, rptr, rdatasize, sendprotocol);
				}

				User_Send_ToOther(_user, sendprotocol, PARTY, rdata, rdatasize, 0, nullptr);
			}
		}
	}
	return false;
}

// 보스 몬스터 이동 정보 및 공격 판단
bool InGameManager::BossMonster_Req_Move(User * _user, char * _buf)
{
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));

	char rdata[BUFSIZE];
	memset(rdata, 0, sizeof(rdata));

	int datasize = 0;
	int rdatasize = 0;

	char* rptr = rdata;

	// 로그용
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	UINT64 sendprotocol = 0;
	UINT64 othersendprotocol = 0;

	int monstercode = 0;
	int monsternum = 0;
	int attackcode = 0;
	float length = 0;
	Vector3 dir;
	PartyRoom* partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());
	Character* character = nullptr;
	MonsterInfo* monster = nullptr;

	if (partyroom == nullptr)
	{
		return false;
	}
	// unpack(코드,방향벡터)
	User_Unpack_BossMonster_Move(_user, _buf, monstercode, monsternum);

	// 시간이 지났으면 새로 정보를 보낸다
	if (User_PartyRoom_Monster_TimeOver_Check(_user, monstercode, monsternum))
	{
		User_Pack_Monster_MoveInfo(_user, monstercode, monsternum, rptr, rdatasize, sendprotocol);
		othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_MOVE_RESULT);

		// 시간 초기화
		User_PartyRoom_Monster_Time_ReSet(_user, monstercode, monsternum);

		// 새로운 타겟을 정한다
		// 제일 가까운 유저가 누구이고 거리가 얼마인가.
		if (GetNearestChracter(_user, monster->GetMonster()->GetMonster_Code(), monster->GetMonsterNum(), length, character))
		{
			User_Pack_Nearset_Character(_user, monstercode, monsternum, character, rptr, rdatasize, sendprotocol);
		}
	}

	// 공격 가능한지 타이머를 확인해보자
	if (partyroom->Is_BossMonster_Attackable() == false)
	{
		User_Send_ToOther(_user, sendprotocol, PARTY, rdata, rdatasize, 0, nullptr);
		return false;
	}

	// 보스 몬스터이면 
	if (partyroom->Is_BossMonster(monstercode))
	{
		// 공격 가능한가(컨트롤러의 보스몬스터가 공격중인지 확인한다)
		// 제일 가까운 유저가 누구이고 거리가 얼마인가.
		if (GetNearestChracter(_user, monstercode, monsternum, length, character))
		{
			return false;
		}
		// 몬스터 정보를 가져온다
		if (GetMonsterInfo(_user, monstercode, monsternum, monster) == false)
		{
			return false;
		}
		// 무슨 공격을 할것인가(타입에 따른 확률을 통해서 무슨 공격할것인지 판단하고 공격을 정하자)
		// 몬스터 정보가 없으면 바로 탈출
		if (partyroom->Is_BossMonster_What_Attack(monster, length, attackcode) == false)
		{
			return false;
		}

		// 공격 정보 패킹하기
		User_Pack_Monster_Attack_Info(_user, rptr, datasize, monstercode, monsternum, attackcode);

		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_BOSS_MONSTER_ATTACK_INFO);

		if (_user->isDungeon())
		{
			User_Send_ToOther(_user, sendprotocol, PARTY, rdata, rdatasize, 0, nullptr);
			return true;
		}
	}
	return false;
}

RESULT InGameManager::InGame_Init_Packet(User * _user)
{
	UINT64 protocol = 0;
	UINT64 compartrprotocol = 0;
	UINT64 tempprotocol = 0;
	char buf[BUFSIZE];
	char rdata[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	memset(rdata, 0, sizeof(rdata));
	int datasize = 0;
	int rdatasize = 0;
	char* ptr = buf;
	char* rptr = rdata;

	char charactercode[CHARACTERCODESIZE];
	memset(charactercode, 0, sizeof(CHARACTERCODESIZE));

	bool check;
	int choice = 0;
	int oldchannelnum = 0;
	int partyroomnum = 0;
	int monstercode = 0;
	int monsternum = 0;
	int attacknum = 0;
	float length = 0;
	SEND_TYPE sendtype = DEFAULT;

	// 로그용
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	_user->BitunPack(protocol, &buf);

	UINT64 sendprotocol = 0;
	UINT64 othersendprotocol = 0;
	RESULT result = RT_DEFAULT;

	compartrprotocol = PROTOCOL_INGAME_CHARACER;

	while (1)
	{
		tempprotocol = 0;

		tempprotocol = protocol & compartrprotocol;
		switch (tempprotocol)
		{
		case PROTOCOL_INGAME_CHARACER:	// 프로토콜 중간틀 캐릭터 관련 이면
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_CHARACTER_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_OTHERPLAYERLIST: // 다른 유저 요청
				User_Pack_OtherUserPosData(_user);
				User_Pack_PlayerPosData(_user, buf, datasize);
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_CONNECT);
				User_Send_ToOther(_user, othersendprotocol, CHANNEL, buf, datasize, 0, nullptr);
				result = RT_INGAME_OTHERPLAYER_LIST;
				break;
			case PROTOCOL_MOVE_REPORT: // 이동 정보
				User_Pack_Move(_user, buf, datasize, rdata, rdatasize);

				// 다른 유저들한테 send
				if (_user->isDungeon())
				{
					sendtype = PARTY_TO_OTHER;
				}
				else
				{
					sendtype = CHANNEL;
				}
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_OTHERPLAYERINFO);
				User_Send_ToOther(_user, othersendprotocol, sendtype, rdata, rdatasize, 0, nullptr);

				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_RESULT);
				_user->Quepack(sendprotocol, buf, datasize);
				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_MOVE_ROTATION: // 회전 정보
				// 다른 유저들한테만 send
				User_Pack_Rotation(_user, buf, datasize);

				// 다른 유저들한테 send
				if (_user->isDungeon())
				{
					sendtype = PARTY_TO_OTHER;
				}
				else
				{
					sendtype = CHANNEL;
				}
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_ROTATION);
				User_Send_ToOther(_user, othersendprotocol, sendtype, buf, datasize, 0, nullptr);
				_user->SetCallback(false);

				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_INGAME_MOVE_START_JUMP: // 점프 정보
				// 캐릭터 정보 점프중이라고 저장
				_user->GetCurCharacter()->SetCharacter_JumpState(true);
				// 다른유저들한테 send
				User_Pack_UserCode(_user, rdata, rdatasize);
				if (_user->isDungeon())
				{
					sendtype = PARTY_TO_OTHER;
				}
				else
				{
					sendtype = CHANNEL;
				}
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_OTHERPLAYER_START_JUMP);
				User_Send_ToOther(_user, othersendprotocol, sendtype, rdata, rdatasize, 0, nullptr);

				//// 메세지
				//memset(msg, 0, sizeof(msg));
				//sprintf(msg, "유저 점프 : 유저가 점프함 [x] :%f [y] :%f [z] :%f ", _user->GetCurCharacter()->GetPosition().x,
				//	_user->GetCurCharacter()->GetPosition().y, _user->GetCurCharacter()->GetPosition().z);
				//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_INGAME_MOVE_END_JUMP: // 착지 정보
				// 캐릭터 정보 점프끝났다고 저장
				_user->GetCurCharacter()->SetCharacter_JumpState(false);
				// 다른유저들한테 send
				User_Pack_UserCode(_user, rdata, rdatasize);
				if (_user->isDungeon())
				{
					sendtype = PARTY_TO_OTHER;
				}
				else
				{
					sendtype = CHANNEL;
				}
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_OTHERPLAYER_END_JUMP);
				User_Send_ToOther(_user, othersendprotocol, sendtype, rdata, rdatasize, 0, nullptr);

				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_INGAME_ATTACK: // 공격했다는 정보
			{
				// 무슨공격을 했는지 알아낸다
				User_Unpack_Attack_Info(_user, buf, attacknum);
				// 그 공격정보를 패킹해서 보내준다.
				User_Pack_AttackNum_Info(_user, rdata, rdatasize, attacknum);
				if (_user->isDungeon())
				{
					sendtype = PARTY_TO_OTHER;
				}
				else
				{
					sendtype = CHANNEL;
				}
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_ATTACK);
				User_Send_ToOther(_user, othersendprotocol, sendtype, rdata, rdatasize, 0, nullptr);

				// 메세지
				//memset(msg, 0, sizeof(msg));
				//sprintf(msg, "유저 공격 : 유저가 공격함 [x] :%f [y] :%f [z] :%f ", _user->GetCurCharacter()->GetPosition().x,
				//	_user->GetCurCharacter()->GetPosition().y, _user->GetCurCharacter()->GetPosition().z);
				//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

				result = RT_INGAME_ATTACK;
				break;
			}
			case PROTOCOL_INGAME_ATTACK_SUCCESS: // 공격 피격판정 확인요청
			{
				User_Attack_Result(_user, buf);
				result = RT_INGAME_ATTACK_RESULT;
				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGMAE_MONSTER: 	// 프로토콜 중간틀 몬스터 관련 이면
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_MONSTER_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_MONSTER_INFO: // 몬스터 정보 요청
			{
				// 스테이지 입장시 몬스터 정보 전송
				User_Dungeon_Stage_Rise(_user);

				User_Pack_Dungeon_Monster_SpawnInfo(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_INFO);
				User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
				_user->Quepack(sendprotocol, buf, datasize);

				result = RT_INGAME_DUNGEON_MONSTER_INFO_RESULT;
				break;
			}
			case PROTOCOL_MONSTER_MOVE: // 몬스터 이동
			{
				PartyRoom* partyroom = nullptr;
				Character* character = nullptr;
				MonsterInfo* monster = nullptr;

				User_Unpack_Monster_Move(_user, buf, monstercode, monsternum);

				if (GetMonsterInfo(_user,monstercode,monsternum,monster))
				{
					if (monster->GetMonsterActivate())
					{
						// 시간이 지났으면 새로 정보를 보낸다
						if (User_PartyRoom_Monster_TimeOver_Check(_user, monstercode, monsternum))
						{
							rptr = rdata;
							User_Pack_Monster_MoveInfo(_user, monstercode, monsternum, rptr, rdatasize, sendprotocol);
							sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_MOVE_RESULT);

							// 시간 초기화
							User_PartyRoom_Monster_Time_ReSet(_user, monstercode, monsternum);

							// 새로운 타겟을 정한다
							// 제일 가까운 유저가 누구이고 거리가 얼마인가.
							if (GetNearestChracter(_user, monstercode, monsternum, length, character))
							{
								User_Pack_Nearset_Character(_user, monstercode, monsternum, character, rptr, rdatasize, sendprotocol);
							}
							User_Send_ToOther(_user, sendprotocol, PARTY, rdata, rdatasize, 0, nullptr);
						}
					}
				}
				
				result = RT_INGMAE_MONSTER_MOVE_RESULT;
				break;
			}
			case PROTOCOL_REQ_MONSTER_ATTACK:	// 몬스터가 유저를 공격했다
			{
				/*if (User_Under_Attack(_user, buf))
				{
					result = RT_INGAME_MONSTER_ATTACK;
				}
				else
				{
					result = RT_INGAME_MONSTER_ATTACK;

				}*/
				result = RT_INGAME_MONSTER_ATTACK;
				break;
			}
			case PROTOCOL_BOSS_MONSTER_MOVE: // 보스 몬스터 이동
			{
				if (BossMonster_Req_Move(_user, buf))
				{
					result = RT_INGAME_MONSTER_ATTACK;
				}
				else
				{
					result = RT_INGAME_MONSTER_ATTACK;
				}
				break;
			}
			case PROTOCOL_REQ_BOSS_MONSTER_ATTACK: // 보스 몬스터가 유저를 공격했다
			{
				// 보스몬스터 공격 정보를 가져온다
				if (User_Under_Attack(_user, buf))
				{
					result = RT_INGAME_MONSTER_ATTACK;
				}
				else
				{
					result = RT_INGAME_MONSTER_ATTACK;

				}

				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_CHANNEL:	// 프로토콜 중간틀 채널 관련 이면
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_CHANNEL_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_CHANNEL_INFO: // 채널 정보 요청
				User_Pack_ChannelInfo(_user);
				result = RT_INGAME_CHANNEL_INFO;
				break;
			case PROTOCOL_REQ_CHANNEL_CHANGE: // 채널 이동 요청
				User_UnPack_ChannelChange(_user, buf, choice);
				if (User_Pack_ChannelChangeResult(_user, buf, choice, datasize, oldchannelnum))
				{
					// 채널 이동 성공
					User_Pack_UserCode(_user, rdata, rdatasize);
					othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHANNEL, PROTOCOL_CHANNLE_USER_CHANGE);
					User_Send_ToOther(_user, othersendprotocol, SPECIFIC_CHANNEL, rdata, rdatasize, oldchannelnum, nullptr);
				}
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHANNEL, PROTOCOL_CHANNLE_CHANGE_RESULT);
				_user->Quepack(sendprotocol, buf, datasize);
				result = RT_INGAME_CHANNEL_CHANGE;
				break;
			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_PARTY:		// 프로토콜 중간틀 파티 관련 이면
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_PARTY_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_PARTY_ROOM_INVITE: // 파티 초대이면
			{
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
					othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_INVITE);
					User_Send_ToOther(_user, othersendprotocol, SPECIFIC_USER, rdata, rdatasize, 0, charactercode);
				}
				else
				{
					// 파티초대 실패 패킹하고 보내준다.
					User_Pack_Party_Result(_user, false, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_INVITE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);
					// 만들어둔 파티방 삭제
					User_Remove_PartyRoom(_user);
				}
				result = RT_INGAME_PARTY_INVITE;
				break;
			}
			case PROTOCOL_PARTY_ROOM_ANSWER_INVITE: // 파티 초대 응답
			{
				User_Unpack_PartyRoom_Invite_Result(_user, buf, check, charactercode, partyroomnum);
				if (check) // 초대 승락
				{
					// 파티 유저 추가
					check = User_Party_AddUser(_user, partyroomnum);
					if (check)
					{
						// 유저 추가 성공
						User_Pack_Party_CharacterInfoToOther(_user, rdata, rdatasize);
						othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_ADD_USER);

						User_Send_ToOther(_user, othersendprotocol, PARTY_TO_OTHER, rdata, rdatasize, 0, nullptr);
						// 나포함 모든 유저들한테 보내는 send함수를 만들자
						_user->Quepack(othersendprotocol, rdata, rdatasize);
					}
					else
					{
						// 유저 추가 실패(초대 받는사람한테 실패라고 보낸다)
						User_Pack_Party_InviteResultToOther(_user, rdata, rdatasize, false);

						sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_JOIN_RESULT);
						_user->Quepack(sendprotocol, buf, datasize);
					}
				}
				else // 초대 거절
				{
					// 파티 초대 거절 패킹하고 보내준다.
					User_Pack_Party_Result(_user, false, rdata, rdatasize);

					othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_INVITE_RESULT);

					User_Send_ToOther(_user, othersendprotocol, SPECIFIC_USER, rdata, rdatasize, 0, charactercode);

					// 유저가 혼자라면 삭제한다.
					if (User_PartyRoom_Alone(partyroomnum))
					{
						// 만들어둔 파티방 삭제
						User_Remove_PartyRoom(charactercode);
					}
				}
				result = RT_INGAME_PARTY_INVITE_RESULT;
				break;
			}
			case PROTOCOL_REQ_USER_KICK: // 파티원 강퇴 요청
			{
				User_Unpack_UserCode(_user, buf, charactercode);
				if (_user->isPartyLeader())
				{
					// 강퇴 성공
					if (User_PartyRoom_User_Kick(_user, charactercode))
					{
						// 강퇴당한 유저한테 알려주기
						User_Pack_Party_Protocol(_user, buf, datasize);
						othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_KICK);
						User_Send_ToOther(_user, othersendprotocol, SPECIFIC_USER, rdata, rdatasize, 0, charactercode);

						// 파티원들 한테는 나갔다라는 코드만 알려주기
						User_Pack_Party_KickInfo(_user, rdata, rdatasize, charactercode);
						othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_KICK_INFO);
						User_Send_ToOther(_user, othersendprotocol, PARTY_TO_OTHER, rdata, rdatasize, 0, nullptr);

						// 파티리더(현재유저)에게는 파티강퇴 실패여부를 알려준다. 그리고 강퇴당한 친구 코드를 넣어준다.
						User_Pack_Party_Result_Code(_user, true, charactercode, buf, datasize);
						sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_KICK_RESULT);
						_user->Quepack(sendprotocol, buf, datasize);
					}
					else
					{
						User_Pack_Party_Result_Code(_user, false, charactercode, buf, datasize);
						sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_KICK_RESULT);
						_user->Quepack(sendprotocol, buf, datasize);
					}
				}
				result = RT_INGAME_PARTY_USER_KICK_RESULT;
				break;
			}
			case PROTOCOL_REQ_USER_LEAVE: // 파티 탈퇴 요청
			{
				if (_user->isPartyLeader())
				{
					// 파티원들한테 파티방 터트렸다는 패킷을보냄
					User_Pack_Party_Protocol(_user, rdata, rdatasize);
					othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_REMOVE_RESULT);
					User_Send_ToOther(_user, othersendprotocol, PARTY_TO_OTHER, rdata, rdatasize, 0, nullptr);

					// 파티없앤다
					check = User_Remove_PartyRoom(_user);

					// 현재유저(파티리더였던유저)에게 파티 탈퇴결과를 보낸다
					User_Pack_Party_Result(_user, check, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);
				}
				else
				{
					// 파티원들한테 파티원 한명 탈퇴했다고 알려준다.
					User_Pack_UserCode(_user, rdata, rdatasize);
					othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_INFO);
					User_Send_ToOther(_user, othersendprotocol, PARTY_TO_OTHER, rdata, rdatasize, 0, nullptr);

					// 파티에서 한명 지운다.
					check = User_PartRoom_User_Leave(_user);

					// 현재유저(파티원이였던유저)에게 파티 탈퇴결과를 보낸다
					User_Pack_Party_Result(_user, check, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);
				}
				result = RT_INGAME_PARTY_USER_LEAVE_RESULT;
				break;
			}
			case PROTOCOL_REQ_LEADER_DELEGATE: // 파티장 위임 요청
			{
				User_Unpack_UserCode(_user, buf, charactercode);

				// 파티장 위임
				if (User_PartyRoom_Leader_Delegate(_user, charactercode))
				{
					// 파티장 위임 성공시 파티리더 바뀐정보를 파티원들한테 보낸다
					User_Pack_PartyRoom_Leader_Delegate(_user, rdata, rdatasize, charactercode);
					othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_LEADER_DELEGATE);
					User_Send_ToOther(_user, othersendprotocol, PARTY_TO_OTHER, rdata, rdatasize, 0, nullptr);

					// 현재 유저에게 파티장 위임 결과를 보낸다.(코드+성공실패)
					User_Pack_Party_Result_Code(_user, true, charactercode, buf, datasize);
				}
				else
				{
					// 현재 유저에게 파티장 위임 결과를 보낸다.(코드+성공실패)
					User_Pack_Party_Result_Code(_user, false, charactercode, buf, datasize);
				}

				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_LEADER_DELEGATE_RESULT);
				_user->Quepack(sendprotocol, buf, datasize);

				result = RT_INGAME_PARTY_LEADER_DELEGATE_RESULT;
				break;
			}

			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_DUNGEON:	// 프로토콜 중간틀 던전 관련 이면
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_DUNGEON_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_DUNGEON_ENTER:// 던전 입장 요청
			{
				User_EnterInDun_Channel(_user);
				// 각 파티원들이 채널에 속해있는 유저들한테 send한다
				User_Send_ToOther(_user, 0, ENTER_DUNGEON, nullptr, 0, 0, nullptr);

				sendprotocol = 0;
				User_Pack_Party_Dungeon_SpawnData(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_ENTER_RESULT);
				User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, datasize, 0, nullptr);
				_user->Quepack(sendprotocol, buf, datasize);

				result = RT_INGAME_DUNGEON_ENTER_RESULT;
				break;
			}
			case PROTOCOL_REQ_DUNGEON_LEAVE: // 던전 나가기 요청
			{
				User_LeaveInDun_Channel(_user);

				result = RT_INGAME_DUNGEON_LEAVE_RESULT;
				break;
			}
			case PROTOCOL_DUNGEON_STAGE_IN: // 스테이지 입장 요청
			{
				// 스테이지 입장시 캐릭터 좌표 전송
				User_Pack_Party_Dungeon_Stage_SpawnData(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_STAGE_IN_RESULT);

				User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, datasize, 0, nullptr);
				_user->Quepack(sendprotocol, buf, datasize);

				// 스테이지 입장시 몬스터 정보 셋팅
				User_Dungeon_Stage_Rise(_user);

				// 몬스터 스폰 정보를 보내주는 스레드 추가
				HANDLE hThread = ThreadManager::GetInstance()->addThread(MonsterSpawnTimerProcess, 0, _user);
				CloseHandle(hThread);

				/*	
				sendprotocol = 0;
				User_Pack_Dungeon_Monster_SpawnInfo(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_INFO);
				User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
				_user->Quepack(sendprotocol, buf, datasize);*/

				result = RT_INGAME_DUNGEON_STAGE_IN_RESULT;
				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGMAE_MENU:		// 프로토콜 중간틀 매뉴 관련 이면
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_MENU_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_MENU_REQ_CHARACTER:// 캐릭터 선택화면으로 요청
			{
				User_Pack_UserCode(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);

				User_Send_LeaveInfoToOther(_user, sendprotocol, buf, datasize);
				User_Leave_Channel(_user);

				sendprotocol = 0;
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MENU, PROTOCOL_MENU_RESULT_CHARACTER);
				_user->Quepack(sendprotocol, buf, 0);
				User_CurCharacter_Save(_user);

				// 현재 접속중인 캐릭터 초기화
				_user->ResetCurCharacter();
				_user->SetLeaveGame();
				_user->SetLeaveDungeon();
				_user->ResetPartyInfo();

				result = RT_INGAME_MENU_CHARACTER;
				break;
			}
			case PROTOCOL_MENU_REQ_LOGOUT: // 로그아웃 요청
			{
				// 유저 코드 패킹
				User_Pack_UserCode(_user, buf, datasize);
				// 다른 유저들한테 send
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
				User_Send_LeaveInfoToOther(_user, othersendprotocol, buf, datasize);
				User_Leave_Channel(_user);

				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MENU, PROTOCOL_MENU_RESULT_LOGOUT);
				_user->Quepack(sendprotocol, buf, 0);
				// 캐릭터 정보 저장
				User_CurCharacter_Save(_user);

				// 유저 정보 초기화
				_user->ResetUserInfo();
				result = RT_INGAME_MENU_LOGOUT;
				break;
			}
			case PROTOCOL_MENU_EXIT:  // 종료 요청
			{
				// 아직 안만듬
				result = RT_INGAME_MENU_EXIT;
				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_ANIMATION: // 프로토콜 중간틀 애니메이션 관련 이면
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_ANIMATION_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_CLIENT_CHARACER_ANIMATION: // 캐릭터 애니메이션
				break;
			case PROTOCOL_CLIENT_MONSTER_ANIMATION: // 몬스터 애니메이션
				break;
			case PROTOCOL_CLIENT_MONSTER_ANIMATION_ATTACK: // 몬스터 공격 애니메이션
			{
				monstercode = 0;
				monsternum = 0;
				attacknum = 0;
				// 무슨공격을 했는지 알아낸다
				//User_Unpack_Monster_Attack_Successfully_The_User(_user, buf, monstercode, monsternum, attacknum);
				// 그 공격정보를 패킹해서 보내준다.
				//User_Pack_Monster_AttackNum_Info(_user, buf, datasize, monstercode, monsternum, attacknum);
				//othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_ANIMATION, PROTOCOL_MONSTER_ANIMATION_ATTACK);
				//if (_user->isDungeon())
				//{
				//	User_Send_Party_ToOther(_user, othersendprotocol, buf, datasize);
				//}

				result = RT_INGAME_MONSTER_ATTACK;
				break;
			}
			default:
				break;
			}
			break;
		default:
			break;
		}

		// 마지막 프로토콜이면 탈출
		if (compartrprotocol == PROTOCOL_INGMAE_MENU)
		{
			break;
		}

		// protocol이랑 비교할 대상 쉬프트 연산
		compartrprotocol = compartrprotocol << 1;
	}

	return result;
}

// 인게임 상태에서 강종했을때 뒷처리
bool InGameManager::User_InGame_Compulsion_Exit(User * _user)
{
	UINT64 sendprotocol = 0;
	UINT64 protocol = 0;
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
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
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_REMOVE_RESULT);
			User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, datasize, 0, nullptr);

			// 파티없앤다
			User_Remove_PartyRoom(_user);
		}
		else
		{
			// 나가는 유저의 정보를 패킹한다
			User_Pack_UserCode(_user, buf, datasize);
			// 파티원, 채널에 접속한 유저들한테 send한다.
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_INFO);
			User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, datasize, 0, nullptr);
			User_PartRoom_User_Leave(_user);
		}

		sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있고 던전에 들어와있는 상태에 종료");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Leave_Channel(_user);

		return true;
	}
	// 파티에 속해있고 던전에 들어와있지않고 채널에 들어와있으면
	else if (_user->isParty() == true && _user->isChannel() == true && _user->isDungeon() == false)
	{
		if (_user->isPartyLeader())
		{
			// 파티원들한테 파티방 터트렸다는 패킷을보냄
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_REMOVE_RESULT);
			User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, 0, 0, nullptr);
			
			User_Pack_UserCode(_user, buf, datasize);
			protocol = _user->BitPackProtocol(protocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
			User_Send_ToOther(_user, protocol, CHANNEL, buf, datasize, 0, nullptr);

			// 파티없앤다
			User_Remove_PartyRoom(_user);

			sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있고 던전에 들어와있지않고 채널에 들어와있는상태에 종료 : 방장임 ");
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
		else
		{
			// 나가는 유저의 정보를 패킹한다
			User_Pack_UserCode(_user, buf, datasize);
			// 파티원, 채널에 접속한 유저들한테 send한다.
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_INFO);
			protocol = _user->BitPackProtocol(protocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);

			User_Send_ToOther(_user, sendprotocol, PARTY_TO_OTHER, buf, datasize, 0, nullptr);
			User_Send_ToOther(_user, protocol, CHANNEL, buf, datasize, 0, nullptr);
			User_PartRoom_User_Leave(_user);

			sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있고 던전에 들어와있지않고 채널에 들어와있는상태에 종료 : 파티원임 ");
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}

		User_Leave_Channel(_user);

		/*sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있고 던전에 들어와있지않고 채널에 들어와있는상태에 종료 ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/
		return true;
	} // 파티에 속해있지않고 채널에 들어와있으면
	else if (_user->isParty() == false && _user->isChannel() == true)
	{
		// 나가는 유저의 정보를 패킹한다
		User_Pack_UserCode(_user, buf, datasize);
		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
		User_Send_ToOther(_user, sendprotocol, CHANNEL, buf, datasize, 0, nullptr);
		User_Leave_Channel(_user);

		sprintf(msg, "User_InGame_Compulsion_Exit :: 파티에 속해있지않고 채널에 들어와있는상태에 종료");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	}

	return false;
}

