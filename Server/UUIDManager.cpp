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

// ĳ���� �ڵ�
void UUIDManager::UUID_CharacterUniqCode(User* _user,char * _codebuf,int _jobcode,int _len,char* _nick)
{
	char jobcodedata[NICKNAMESIZE];

	memset(jobcodedata, 0, sizeof(NICKNAMESIZE));

	char* codedata = _codebuf;

	// �����ڵ� ����� (�������̵�[20] + ĳ���ʹг���[6] + �����ڵ�[4])
	itoa(_jobcode, jobcodedata, 10);

	// �����ڵ忡 �������̵� �߰�
	memcpy(codedata, _user->getID(), strlen(_user->getID()));
	codedata += strlen(_user->getID());

	// ĳ���� �г��ӱ��̰� 6���� ������ ���̸�ŭ�ؼ� �ڵ常���
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

	// ���������
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
