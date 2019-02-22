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

// ���� ����
void InGameManager::EndManager()
{
}

// ������ ��������Ʈ ����
void InGameManager::User_Pack_OtherUserPosData(User * _user)
{
	// �ΰ��ӿ� �������� ���� ����Ʈ(ī��Ʈ, ���� �ڵ�, �г���, ��ġ...rep)
	// 
	// count�ڸ� ����α�. ���߿� count �־��ٶ��� ptr ����ϱ�
	PROTOCOL sendprotocol;
	char data[BUFSIZE];
	char* ptr = data;
	int size = 0;

	// ī��Ʈ �ڸ� �����
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
		// ���� �˻�
		if (UserManager::GetInstance()->searchData(user_temp) == true)
		{
			if (_user->getSocket() != user_temp->getSocket() && user_temp->isIngame() == true)
			{
				character_temp = user_temp->GetCurCharacter();
				character_jobcode = character_temp->GetCharacter_JobCode();
				nicksize = strlen(character_temp->GetCharacter_Name());
				codesize = strlen(character_temp->GetCharacter_Code());

				// �ڵ� ������
				memcpy(ptr, &codesize, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);

				// �ڵ�
				memcpy(ptr, character_temp->GetCharacter_Code(), codesize);
				ptr += codesize;
				size += codesize;

				// ���� �ڵ�
				memcpy(ptr, &character_jobcode, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);
				// �г��� ������
				memcpy(ptr, &nicksize, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);
				// �г���
				memcpy(ptr, character_temp->GetCharacter_Name(), nicksize);
				ptr += nicksize;
				size += nicksize;
				// ��ġ
				memcpy(ptr, &character_temp->GetPosition(), sizeof(Vector3));
				ptr += sizeof(Vector3);
				size += sizeof(Vector3);
				// ȸ��
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
	sprintf(msg, "��������Ʈ ���� %d��", usercount);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	memcpy(data, &usercount, sizeof(int));
	size += sizeof(int);

	sendprotocol = SEVER_INGAME_OTHERPLAYERLIST_RESULT;
	_user->pack(sendprotocol, data, size);
}

// �̵� �Ϸ� ��û
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

	// ���� ������ ����
	prePos = _user->GetCurCharacter()->GetPosition();

	// ��ġ ȸ�� �ð� ����
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	memcpy(&curRot, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	//
	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "�̵� �Ϸ� ������ :: ��ġ: %f %f %f ȸ��: %f %f %f ", curPos.x, curPos.y,
		curPos.z, curRot.x, curRot.y, curRot.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// ���� Ŭ����
	ptr = _buf;
	memset(_buf, 0, sizeof(_buf));

	// �̵�����
	//
	//
	lesult = true;

	// ���
	memcpy(ptr, &lesult, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// �������̵�
	if (lesult == false)
	{
		// ��ġ
		memcpy(ptr, &prePos, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr += sizeof(Vector3);

		// ȸ��
		memcpy(ptr, &curRot, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr += sizeof(Vector3);

		// ���� x
		memcpy(ptr, &dirx, sizeof(float));
		datasize += sizeof(float);
		ptr += sizeof(float);

		// ���� y
		memcpy(ptr, &diry, sizeof(float));
		datasize += sizeof(float);
		ptr += sizeof(float);

		_user->GetCurCharacter()->SetPosition(curRot);
	}
	else	 // �����̵�
	{
		_user->GetCurCharacter()->SetPosition(curRot);
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// �ٸ��������� �� ��������
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// �ٸ��������� �� ���� ������
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize, dirx, diry);

	// �ܺο� ����
	memcpy(_releasedata, relesedata, rdatasize);
	_rdatasize = rdatasize;

	return lesult;
}

// �̵� ����
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

	// ���� ��ġ ����
	prePos = _user->GetCurCharacter()->GetPosition();

	// ���� ��ġ ����
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	memcpy(&curRot, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "�̵� ���� ������ :: ��ġ: %f %f %f ȸ��: %f %f %f", curPos.x, curPos.y,
		curPos.z, curRot.x, curRot.y, curRot.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// �ʱ�ȭ
	ptr = _buf;
	memset(_buf, 0, sizeof(_buf));

	// �̵�����
	//
	//
	lesult = true;

	// ���
	memcpy(ptr, &lesult, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// �������̵�
	if (lesult == false)
	{
		// ��ġ
		memcpy(ptr, &prePos, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr += sizeof(Vector3);

		// ȸ��
		memcpy(ptr, &curRot, sizeof(Vector3));
		datasize += sizeof(Vector3);
		ptr += sizeof(Vector3);

		// ���� x
		memcpy(ptr, &dirx, sizeof(float));
		datasize += sizeof(float);
		ptr += sizeof(float);

		// ���� y
		memcpy(ptr, &diry, sizeof(float));
		datasize += sizeof(float);
		ptr += sizeof(float);

		_user->GetCurCharacter()->SetPosition(curRot);
	}
	else	 // �����̵�
	{
		_user->GetCurCharacter()->SetPosition(curRot);
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// �ٸ��������� �� ��������
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// �ٸ��������� �� ���� ������
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize, dirx, diry);

	// �ܺο� ����
	memcpy(_releasedata, relesedata, rdatasize);
	_rdatasize = rdatasize;

	return lesult;
}

// �ٸ��������� �� �������� ������ ��ŷ
void InGameManager::User_Pack_MoveInfoToOther(User* _user, char * _data, int & _datasize, float _dirx, float _diry)
{
	int datasize = 0;
	int len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// �ڵ� ������
	memcpy(ptr, &len, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �ڵ�
	memcpy(ptr, curcharacter->GetCharacter_Code(), len);
	datasize += len;
	ptr += len;

	// ��ġ
	memcpy(ptr, &curcharacter->GetPosition(), sizeof(Vector3));
	datasize += sizeof(Vector3);
	ptr += sizeof(Vector3);

	// ȸ��
	memcpy(ptr, &curcharacter->GetRotation(), sizeof(Vector3));
	datasize += sizeof(Vector3);
	ptr += sizeof(Vector3);

	// ���� x
	memcpy(ptr, &_dirx, sizeof(float));
	datasize += sizeof(float);
	ptr += sizeof(float);

	// ���� y
	memcpy(ptr, &_diry, sizeof(float));
	datasize += sizeof(float);
	ptr += sizeof(float);

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "�ٸ� �������� ������ :: ��ġ: %f %f %f ȸ��: %f %f %f ����: %f %f", curcharacter->GetPosition().x, curcharacter->GetPosition().y,
		curcharacter->GetPosition().z, curcharacter->GetRotation().x, curcharacter->GetRotation().y, curcharacter->GetRotation().z, _dirx, _diry);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// ������ġ �ٸ��������� ��������
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
	
	// ��������
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
