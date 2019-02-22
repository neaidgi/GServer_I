#include "InGameManager.h"

InGameManager* InGameManager::Instance = nullptr;

InGameManager::InGameManager()
{
	verification = new CharacterVerification();
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
	//verification->Initalize();
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

	User* user_temp;
	Character* character_temp;
	UserManager::GetInstance()->startSearch();
	while (1)
	{
		// 유저 검색
		if (UserManager::GetInstance()->searchData(user_temp) == true)
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

	sendprotocol = SEVER_INGAME_OTHERPLAYERLIST_RESULT;
	_user->pack(sendprotocol, data, size);
}

// 이동 완료 요청
bool InGameManager::User_Pack_Move(User * _user, char* _buf, int& _datasize, char * _releasedata, int& _rdatasize)
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

	// 받은 데이터 복사
	prePos = _user->GetCurCharacter()->GetPosition();

	// 위치 회전 시간 복사
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	memcpy(&curRot, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	//
	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "이동 완료 데이터 :: 위치: %f %f %f 회전: %f %f %f ", curPos.x, curPos.y,
		curPos.z, curRot.x, curRot.y, curRot.z);
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

		_user->GetCurCharacter()->SetPosition(curRot);
	}
	else	 // 정상이동
	{
		_user->GetCurCharacter()->SetPosition(curRot);
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// 다른유저에게 줄 정보변수
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// 다른유저에게 줄 정보 데이터
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize, dirx, diry);

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
	sprintf(msg, "이동 시작 데이터 :: 위치: %f %f %f 회전: %f %f %f", curPos.x, curPos.y,
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

		_user->GetCurCharacter()->SetPosition(curRot);
	}
	else	 // 정상이동
	{
		_user->GetCurCharacter()->SetPosition(curRot);
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// 다른유저에게 줄 정보변수
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// 다른유저에게 줄 정보 데이터
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize, dirx, diry);

	// 외부에 복사
	memcpy(_releasedata, relesedata, rdatasize);
	_rdatasize = rdatasize;

	return lesult;
}

// 다른유저에게 줄 유저정보 데이터 패킹
void InGameManager::User_Pack_MoveInfoToOther(User* _user, char * _data, int & _datasize, float _dirx, float _diry)
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

	// 회전
	memcpy(ptr, &curcharacter->GetRotation(), sizeof(Vector3));
	datasize += sizeof(Vector3);
	ptr += sizeof(Vector3);

	// 방향 x
	memcpy(ptr, &_dirx, sizeof(float));
	datasize += sizeof(float);
	ptr += sizeof(float);

	// 방향 y
	memcpy(ptr, &_diry, sizeof(float));
	datasize += sizeof(float);
	ptr += sizeof(float);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "다른 유저전송 데이터 :: 위치: %f %f %f 회전: %f %f %f 방향: %f %f", curcharacter->GetPosition().x, curcharacter->GetPosition().y,
		curcharacter->GetPosition().z, curcharacter->GetRotation().x, curcharacter->GetRotation().y, curcharacter->GetRotation().z, _dirx, _diry);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// 유저위치 다른유저에게 전송해줌
void InGameManager::User_Send_MoveInfoToOther(User* _user, PROTOCOL _p, char * _data, int & _datasize)
{
	char* ptr = _data;

	User* user;
	while (UserManager::GetInstance()->searchData(user))
	{
		if (user->isIngame() && user->getSocket() != _user->getSocket())
		{
			user->pack(_p, ptr, _datasize);
			user->IOCP_OneSided_SendMsg();
		}
	}
}

RESULT InGameManager::InGame_Init_Packet(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char rdata[BUFSIZE];
	char* ptr = buf;
	bool check;
	int choice;
	int datasize = 0;
	int rdatasize = 0;

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result;
	
	// 수정했음
	switch (protocol)
	{
	case CLIENT_INGAME_OTHERPLAYERLIST:
		User_Pack_OtherUserPosData(_user);
		result = RT_INGAME_OTHERPLAYER_LIST;
		break;
	case CLIENT_INGAME_MOVE_START:
		if (User_Pack_MoveStart(_user, buf, datasize, rdata, rdatasize))
		{

		}
		else
		{

		}
		sendprotocol = SEVER_INGAME_MOVE_RESULT;
		_user->pack(sendprotocol, buf, datasize);
		result = RT_INGAME_MOVE;
		User_Send_MoveInfoToOther(_user, SEVER_INGAME_MOVE_ORDER,rdata, rdatasize);
		break;
	case CLIENT_INGAME_MOVE_REPORT:
		if (User_Pack_Move(_user, buf, datasize, rdata, rdatasize))
		{

		}
		else
		{

		}
		sendprotocol = SEVER_INGAME_MOVE_RESULT;
		_user->pack(sendprotocol, buf, datasize);
		result = RT_INGAME_MOVE;
		User_Send_MoveInfoToOther(_user, SEVER_INGAME_MOVE_OTHERPLAYERINFO,rdata, rdatasize);
		break;
	case CLIENT_INGAME_MOVE_END:
		if (User_Pack_Move(_user, buf, datasize, rdata, rdatasize))
		{

		}
		else
		{

		}
		sendprotocol = SEVER_INGAME_MOVE_RESULT;
		_user->pack(sendprotocol, buf, datasize);
		result = RT_INGAME_MOVE;
		User_Send_MoveInfoToOther(_user, SEVER_INGAME_MOVE_OTHERPLAYERINFO, rdata, rdatasize);
		break;
	default:
		break;
	}

	return result;
}
