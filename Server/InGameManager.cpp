#include "InGameManager.h"

InGameManager* InGameManager::Instance = nullptr;

InGameManager::InGameManager()
{
	verification = nullptr;
	channelsystem = nullptr;
}

InGameManager::~InGameManager()
{
	delete verification;
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
	// 인증, 채널시스템 초기화
	verification = new CharacterVerification();
	channelsystem = new ChannelSystem();

	channelsystem->InitializeChannel();
	return true;
}

// 서버 종료
void InGameManager::EndManager()
{
	delete channelsystem;
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

// 인게임에서 떠날때 유저의 캐릭터 코드 패킹
void InGameManager::User_Pack_Leave_InGame(User * _user, char * _data, int & _datasize)
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
	sprintf(msg, "User_Pack_Leave_InGame :: 떠나는 유저의 캐릭터 코드 : [%s] ", curcharacter->GetCharacter_Code());
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

// 캐릭터 정보 언팩 - 현재 사용안함
void InGameManager::User_Unpack_CharacterInfo(User * _user, char * _buf)
{

}

// 채널 이동 요청 언팩
void InGameManager::User_UnPack_ChannelChange(User * _user, char * _buf, int& _channelnum)
{
	int channelnum = 0;

	memcpy(&channelnum, _buf, sizeof(int));

	_channelnum = channelnum;
}

// 다른유저에게 줄 유저정보 데이터 패킹
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

// 던전 채널 나가기
void InGameManager::User_LeaveInDun_Channel(User * _user)
{
	if (channelsystem->DungeonLeave(_user->GetPartyNum()))
	{

	}
	else
	{

	}

	// 마을 채널 들어가기
	if (User_Enter_Channel(_user))
	{

	}
	else
	{

	}
}

// 던전 채널 들어가기
void InGameManager::User_EnterInDun_Channel(User * _user)
{
	// 파티 검색

	// 파티 채널추가

}

// 다른유저에게 전송해줌
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
	User* user = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	if (_user->isParty())
	{
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
				sprintf(msg, "User_Send_LeaveInfoToOther :: 보내는 소켓: [%d] 받는 소켓: [%d] 아이디: [%s] 프로토콜: [%d]\n 데이터사이즈: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
					_p, _datasize, user->GetSendQueueSize());
				MsgManager::GetInstance()->DisplayMsg("INFO", msg);
			}
		}

		User_Leave_Channel(_user);
	}
	else
	{
		// 파티속해있는 유저들한테 전송
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


RESULT InGameManager::InGame_Init_Packet(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char rdata[BUFSIZE];
	char* ptr = buf;
	bool check;
	int choice = 0;
	int oldchannelnum = 0;
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
			User_Pack_Leave_InGame(_user, rdata, rdatasize);
			User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, oldchannelnum, rdata, rdatasize);
		}

		sendprotocol = SERVER_INGAME_CHANNLE_CHANGE_RESULT;
		_user->Quepack(sendprotocol, buf, datasize);

		result = RT_INGAME_CHANNEL_CHANGE;
		break;
	case CLIENT_INGAME_MENU_REQ_CHARACTER:	// 캐릭터 선택화면 요청
		User_Pack_Leave_InGame(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);

		sendprotocol = SERVER_INGAME_MENU_RESULT_CHARACTER;
		_user->Quepack(sendprotocol, buf, 0);
		User_CurCharacter_Save(_user);

		// 현재 접속중인 캐릭터 초기화
		_user->ResetCurCharacter();
		_user->SetLeaveGame();

		result = RT_INGAME_MENU_CHARACTER;
		break;
	case CLIENT_INGAME_MENU_REQ_LOGOUT:		// 로그아웃 요청
		User_Pack_Leave_InGame(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);

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
	default:
		break;
	}

	return result;
}
