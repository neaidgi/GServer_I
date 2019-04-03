#include "UUIDManager.h"

UUIDManager* UUIDManager::Instance = nullptr;

UUIDManager::UUIDManager()
{
	
}

UUIDManager::~UUIDManager()
{

}

void UUIDManager::CreateInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}

	Instance = new UUIDManager();
}

UUIDManager * UUIDManager::GetInstance()
{
	return Instance;
}

void UUIDManager::DestroyInstance()
{
	delete Instance;
}

bool UUIDManager::InitializeManager()
{
	return true;
}

void UUIDManager::EndManager()
{

}

bool UUIDManager::isPartyRoomNum(int _partyroomnum)
{
	int num = 0;

	for (save = partyroomnum.begin(); save != partyroomnum.end(); save++)
	{
		num = (*save);
		if (_partyroomnum == num)
		{
			return true;
		}
	}
	return false;
}

// 캐릭터 코드
void UUIDManager::UUID_CharacterUniqCode(User* _user,char * _codebuf,int _jobcode,int _len,char* _nick)
{
	char jobcodedata[NICKNAMESIZE];

	memset(jobcodedata, 0, sizeof(NICKNAMESIZE));

	char* codedata = _codebuf;

	// 고유코드 만들기 (유저아이디[20] + 캐릭터닉네임[6] + 직업코드[4])
	itoa(_jobcode, jobcodedata, 10);

	// 고유코드에 유저아이디 추가
	memcpy(codedata, _user->getID(), strlen(_user->getID()));
	codedata += strlen(_user->getID());

	// 캐릭터 닉네임길이가 6보다 작으면 길이만큼해서 코드만들기
	if (_len >= NICKNAME_CODESIZE)
	{
		memcpy(codedata, _nick, NICKNAME_CODESIZE);
		codedata += NICKNAME_CODESIZE;
	}
	else
	{
		memcpy(codedata, _nick, _len - 1);
		codedata += _len - 1;
	}

	memcpy(codedata, jobcodedata, JOBCODE_SIZE);
	codedata += JOBCODE_SIZE;

	printf("nick %s \n", _nick);
	printf("uniqcode %s \n", _codebuf);
}

int UUIDManager::UUID_PartyRoomNum()
{
	CriticalSectionManager::GetInstance()->Enter();
	int num = 0;

	// 비어있으면
	if (partyroomnum.empty() == true)
	{
		partyroomnum.push_back(num);

		CriticalSectionManager::GetInstance()->Leave();
		return num;
	}

	num = partyroomnum.back();

	num++;

	partyroomnum.push_back(num);

	CriticalSectionManager::GetInstance()->Leave();
	return num;
}
