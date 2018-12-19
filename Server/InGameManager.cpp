#include "InGameManager.h"

InGameManager* InGameManager::Instance = nullptr;

InGameManager::InGameManager()
{

}

InGameManager::~InGameManager()
{

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
	return true;
}

// 서버 종료
void InGameManager::EndManager()
{
}

void InGameManager::User_Req_OtherUserPosData(User * _user)
{
	// 인게임에 접속중인 유저 리스트(카운트, 캐릭터 코드, 닉네임, 위치...rep)
	// 
	// count자리 비워두기. 나중에 count 넣어줄때는 ptr 사용하기
	PROTOCOL sendprotocol;
	char data[BUFSIZE];
	char* ptr = data;
	int size = 0;

	// 카운트 자리 남기기
	ptr += sizeof(int);

	int usercount = 0;
	int character_uniquecode;
	int character_code;
	int nicksize;

	User* user_temp;
	Character* character_temp;
	UserManager::GetInstance()->startSearch();
	while (1)
	{
		if (UserManager::GetInstance()->searchData(user_temp) == true)
		{
			if (user_temp->isIngame() == true)
			{
				character_temp = user_temp->GetCurCharacter();
				character_uniquecode = character_temp->GetCharacter_UniqueCode();
				character_code = character_temp->GetCharacter_Code();
				nicksize = strlen(character_temp->GetCharacter_Name());

				// 캐릭터 코드
				memcpy(ptr, &character_code, sizeof(int));
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

				usercount++;
			}
		}
		else
		{
			break;
		}
	}

	memcpy(ptr, &usercount, sizeof(int));
	size += sizeof(int);

	sendprotocol = SEVER_INGAME_OTHERPLAYERLIST_RESULT;
	_user->pack(sendprotocol, data, size);
}

RESULT InGameManager::InGame_Init_Packet(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char* ptr = buf;
	bool check;
	int choice;

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
	default:
		break;
	}

	return result;
}
