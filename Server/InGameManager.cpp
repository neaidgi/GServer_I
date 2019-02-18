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

void InGameManager::User_Req_OtherUserPosData(User * _user)
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
	int nicksize;

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

bool InGameManager::User_Req_Move(User * _user, char* _buf, int& _datasize)
{
	Vector3 prePos;
	Vector3 curPos;
	Vector3 curRot;
	bool lesult;
	int datasize;
	int len;
	char data[BUFSIZE];
	char* ptr = _buf;
	char* ptr_data = data;

	memset(data, 0, sizeof(data));

	prePos = _user->GetCurCharacter()->GetPosition();
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	memcpy(&curRot, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	// 이동검증
	//
	//
	lesult = true;

	_user->GetCurCharacter()->SetPosition(curPos);
	_user->GetCurCharacter()->SetRotation(curRot);

	memcpy(ptr_data, &lesult, sizeof(bool));
	datasize += sizeof(bool);
	ptr_data += sizeof(bool);

	// 정상이동
	if (lesult)
	{
		// 닉네임 사이즈
		len = strlen(_user->GetCurCharacter()->GetCharacter_Name());
		memcpy(ptr_data, &len, sizeof(int));
		datasize += sizeof(int);
		ptr_data += sizeof(int);

		// 닉네임
		memcpy(ptr_data, _user->GetCurCharacter()->GetCharacter_Name(), len);
		datasize += len;
		ptr_data += len;

		// 위치
		memcpy(ptr_data, &curPos, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr_data += sizeof(Vector3);

		// 방향
		memcpy(ptr_data, &curRot, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr_data += sizeof(Vector3);

		return true;
	}
	// 비정상이동
	else
	{
		// 닉네임 사이즈
		len = strlen(_user->GetCurCharacter()->GetCharacter_Name());
		memcpy(ptr_data, &len, sizeof(int));
		datasize += sizeof(int);
		ptr_data += sizeof(int);

		// 닉네임
		memcpy(ptr_data, _user->GetCurCharacter()->GetCharacter_Name(), len);
		datasize += len;
		ptr_data += len;

		// 위치
		memcpy(ptr_data, &prePos, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr_data += sizeof(Vector3);

		// 방향
		memcpy(ptr_data, &curRot, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr_data += sizeof(Vector3);

		return false;
	}
	
	_datasize = datasize;
}

// 유저위치 다른유저에게 전송해줌
void InGameManager::User_Send_MoveInfotoOther(User* _user, char * _data, int & _datasize)
{
	char* ptr = _data;

	// 첫번째 값 결과는 무시
	ptr += sizeof(bool);
	_datasize -= sizeof(bool);

	User* user;
	while (UserManager::GetInstance()->searchData(user))
	{
		if (user->isIngame() && user->getSocket() != _user->getSocket())
		{
			user->pack(SEVER_INGAME_OTHERPLAYER_INFO, ptr, _datasize);
			user->IOCP_OneSided_SendMsg();
		}
	}
}

RESULT InGameManager::InGame_Init_Packet(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char* ptr = buf;
	bool check;
	int choice;
	int datasize;

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_INGAME_OTHERPLAYERLIST:
		User_Req_OtherUserPosData(_user);
		result = RT_INGAME_OTHERPLAYER_LIST;
		break;
	case CLIENT_INGAME_MOVE:
		if (User_Req_Move(_user, buf, datasize))
		{
			sendprotocol = SEVER_INGAME_MOVE_RESULT;
			_user->pack(sendprotocol, buf, datasize);
			result = RT_INGAME_MOVE;
			User_Send_MoveInfotoOther(_user, buf, datasize);
		}
		else
		{

		}
	default:
		break;
	}

	return result;
}
