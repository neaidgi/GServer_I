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
	// ����, ä�νý���,��Ƽ�ý��� �ʱ�ȭ
	verification = new CharacterVerification();
	channelsystem = new ChannelSystem();
	m_partysystem = new PartySystem();

	channelsystem->InitializeChannel();
	m_partysystem->InitializePartySystem();
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

	User* user_temp = nullptr;
	Character* character_temp;

	CriticalSectionManager::GetInstance()->Enter();

	// ä�ο� �����ִ� ���� ���� ��ŷ
	channelsystem->StartSearchTownUser(_user->GetChannelNum());

	while (1)
	{
		// ���� �˻�
		if (channelsystem->SearchTownUser(_user->GetChannelNum(), user_temp) == true)
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

	sendprotocol = SERVER_INGAME_OTHERPLAYERLIST_RESULT;
	_user->Quepack(sendprotocol, data, size);
	CriticalSectionManager::GetInstance()->Leave();
}

// ������ ���� ���� ��ŷ
void InGameManager::User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize)
{
	PROTOCOL sendprotocol;
	char* ptr = _data;
	int size = 0;
	int len = 0;

	memset(ptr, 0, BUFSIZE);

	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	// �ڵ� ������
	len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	sprintf(msg, "������ ���� ������ :: �ڵ������: %d ", len);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// �ڵ�
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), len);
	ptr += len;
	size += len;

	sprintf(msg, "�ڵ�: [%s] ", _user->GetCurCharacter()->GetCharacter_Code());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// ���� 
	int jobcode = _user->GetCurCharacter()->GetCharacter_JobCode();
	memcpy(ptr, &jobcode, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	sprintf(msg, "�����ڵ�: [%d] ", jobcode);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// �г��� ������
	len = strlen(_user->GetCurCharacter()->GetCharacter_Name());
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	sprintf(msg, "�г��ӻ�����: [%d] ", len);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// �г���
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Name(), len);
	ptr += len;
	size += len;

	sprintf(msg, "�г���: [%s]\n", _user->GetCurCharacter()->GetCharacter_Name());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// ��ġ
	memcpy(ptr, &_user->GetCurCharacter()->GetPosition(), sizeof(Vector3));
	ptr += sizeof(Vector3);
	size += sizeof(Vector3);

	sprintf(msg, "��ġ: [%f] [%f] [%f] ", _user->GetCurCharacter()->GetPosition().x, _user->GetCurCharacter()->GetPosition().y, _user->GetCurCharacter()->GetPosition().z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// ȸ��
	memcpy(ptr, &_user->GetCurCharacter()->GetRotation(), sizeof(Vector3));
	ptr += sizeof(Vector3);
	size += sizeof(Vector3);

	sprintf(msg, "ȸ��: [%f] [%f] [%f] ", _user->GetCurCharacter()->GetRotation().x, _user->GetCurCharacter()->GetRotation().y, _user->GetCurCharacter()->GetRotation().z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = size;
}

// �̵� �Ϸ� ��û
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

	// ���� ������ ����
	prePos = _user->GetCurCharacter()->GetPosition();

	// ��ġ ����
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "�̵� �Ϸ� ������ :: ���̵�: [%s] ��ġ: [%f] [%f] [%f]", _user->getID(), curPos.x, curPos.y,
		curPos.z);
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
	}
	else	 // �����̵�
	{
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// �ٸ��������� �� ��������
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// �ٸ��������� �� ���� ������
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize);

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
	sprintf(msg, "�̵� �Ϸ� ������ :: ���̵�: [%s] ��ġ: [%f] [%f] [%f]", _user->getID(), curPos.x, curPos.y,
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

		_user->GetCurCharacter()->SetRotation(curRot);
	}
	else	 // �����̵�
	{
		_user->GetCurCharacter()->SetRotation(curRot);
		_user->GetCurCharacter()->SetPosition(curPos);
	}

	_datasize = datasize;

	// �ٸ��������� �� ��������
	char relesedata[BUFSIZE];
	int rdatasize = 0;

	// �ٸ��������� �� ���� ������
	User_Pack_MoveInfoToOther(_user, relesedata, rdatasize);

	// �ܺο� ����
	memcpy(_releasedata, relesedata, rdatasize);
	_rdatasize = rdatasize;

	return lesult;
}

// ���� ȸ�� ��ŷ
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

	// �ڵ� ������
	len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	datasize += sizeof(int);
	// �ڵ�
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), len);
	ptr += len;
	datasize += len;
	// ȸ����
	memcpy(ptr, &curRot, sizeof(Vector3));
	ptr += sizeof(Vector3);
	datasize += sizeof(Vector3);

	_datasize = datasize;

	// �޼���
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "ȸ�� ������ :: ���̵�: [%s] ȸ��: [%f] [%f] [%f]", _user->getID(), curRot.x, curRot.y,
		curRot.z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

}

// ���� �ڵ� ��ŷ
void InGameManager::User_Pack_UserCode(User * _user, char * _data, int & _datasize)
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

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_UserCode :: ������ ������ ĳ���� �ڵ� : [%s] ", curcharacter->GetCharacter_Code());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// ä�� ���� ����
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
		// ä�� ��ȣ
		memcpy(ptr, &i, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// ä�ο� ������
		usercount = channelsystem->GetChannelSize(i);

		memcpy(ptr, &usercount, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		memset(msg, 0, sizeof(msg));
		sprintf(msg, "ä�� ���� ���� ä�ι�ȣ : [%d] ������ : [%d]", i, usercount);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	sendprotocol = SERVER_INGAME_CHANNLE_INFO_RESULT;
	_user->Quepack(sendprotocol, data, size);

	CriticalSectionManager::GetInstance()->Leave();
}

// ä�� �̵� ��� ��ŷ
bool InGameManager::User_Pack_ChannelChangeResult(User * _user, char* _buf, int _choice, int& _datasize, int& _oldchannelnum)
{
	int oldchannelnum = 0;
	bool result;
	int size = 0;
	char* ptr = _buf;

	char msg[BUFSIZE];

	// ä�� ��ȣ �ӽ�����
	oldchannelnum = _user->GetChannelNum();

	memset(_buf, 0, BUFSIZE);
	ptr = _buf;

	CriticalSectionManager::GetInstance()->Enter();

	if (User_Enter_Channel(_user, _choice))
	{
		result = true;
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "ä�� �̵����� ä�ι�ȣ : [%d] ", _choice);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		// ������ ä�ο��� ������
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
		sprintf(msg, "ä�� �̵����� ä�ι�ȣ : [%d] ", _choice);
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

// ���� ������ ä�θ������ ��ŷ
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

// ��Ƽ �ʴ� ��ŷ(�ڵ�,�г���,��Ƽ��ȣ)
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

	// �ڵ� ������
	memcpy(ptr, &codelen, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �ڵ�
	memcpy(ptr, curcharacter->GetCharacter_Code(), codelen);
	datasize += codelen;
	ptr += codelen;

	// �г��� ������
	memcpy(ptr, &nicklen, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �г���
	memcpy(ptr, curcharacter->GetCharacter_Name(), nicklen);
	datasize += nicklen;
	ptr += nicklen;

	// ��Ƽ ��ȣ
	memcpy(ptr, &partynum, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_Invite_Other :: ��Ƽ��ȣ : [%d] �ʴ��ϴ� ���� �ڵ� : [%s] �ʴ��ϴ� ���� �г��� : [%s] ", _user->GetPartyNum(), curcharacter->GetCharacter_Code(), curcharacter->GetCharacter_Name());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;

	//CriticalSectionManager::GetInstance()->Leave();
}

// ��Ƽ�� �����ִ� �������� ��ŷ(��Ƽ��ȣ,��������,�ڵ�,�����ڵ�,�г���,Hp,Mp,��������)
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

	// ��Ƽ��
	partyroom = m_partysystem->GetPartyRoomSearch(partynum);
	partyusercount = m_partysystem->GetPartyRoomUserNum(partynum);

	// ��Ƽ ��ȣ
	memcpy(ptr, &partynum, sizeof(int));
	ptr += sizeof(int);
	datasize += sizeof(int);

	// ���� ����
	memcpy(ptr, &partyusercount, sizeof(int));
	ptr += sizeof(int);
	datasize += sizeof(int);

	CriticalSectionManager::GetInstance()->Enter();

	// ��Ƽ�� �˻� ����
	partyroom->StartSearchPartyRoom();

	while (1)
	{
		// ���� �˻�
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

				// �ڵ� ������
				memcpy(ptr, &codelen, sizeof(int));
				ptr += sizeof(int);
				datasize += sizeof(int);

				// �ڵ�
				memcpy(ptr, character_temp->GetCharacter_Code(), codelen);
				ptr += codelen;
				datasize += codelen;

				// ���� �ڵ�
				memcpy(ptr, &character_jobcode, sizeof(int));
				ptr += sizeof(int);
				datasize += sizeof(int);

				// �г��� ������
				memcpy(ptr, &nicklen, sizeof(int));
				ptr += sizeof(int);
				datasize += sizeof(int);

				// �г���
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

				// ��������
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

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_InviteResultToOther :: ��Ƽ��ȣ : [%d] ��Ƽ�ο� [%d]", _user->GetPartyNum(), partyusercount);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;

	CriticalSectionManager::GetInstance()->Leave();
}

// ��Ƽ �ʴ��ߴµ� �濡 ��������
void InGameManager::User_Pack_Party_InviteResultToOther(User * _user, char * _data, int & _datasize)
{
	int datasize = 0;
	char* ptr = _data;
	char msg[BUFSIZE];
	bool result = false;

	// ���
	memcpy(ptr, &result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_InviteResultToOther ::  ");
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// ��Ƽ �������� ��ŷ (��������)
void InGameManager::User_Pack_Party_Protocol(User * _user, char * _data, int & _datasize)
{
	int datasize = 0;
	char* ptr = _data;

	char msg[BUFSIZE];

	_datasize = datasize;
}

// ��Ƽ ���� ���� ���� ���� ��ŷ. (Ư�� ������ �ڵ�)
void InGameManager::User_Pack_Party_KickInfo(User * _user, char * _data, int & _datasize, char * _code)
{
	int datasize = 0;
	int len = strlen(_code);
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// �ڵ� ������
	memcpy(ptr, &len, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �ڵ�
	memcpy(ptr, _code, len);
	datasize += len;
	ptr += len;

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Party_KickInfo :: ������� ������ ĳ���� �ڵ� : [%s] ", _code);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// ��Ƽ ���� �ٲ� ���� ���� ��Ŷ. (�ڵ�, �ڵ�)
void InGameManager::User_Pack_PartyRoom_Leader_Delegate(User * _user, char * _data, int & _datasize, char * _code)
{
	int datasize = 0;
	int oldlne = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	int len = strlen(_code);
	char* ptr = _data;
	char msg[BUFSIZE];

	Character* curcharacter = _user->GetCurCharacter();

	// �� ��Ƽ ���� �ڵ� ������
	memcpy(ptr, &oldlne, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �� ��Ƽ ���� �ڵ�
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), oldlne);
	datasize += oldlne;
	ptr += oldlne;

	// �� ��Ƽ ���� �ڵ� ������
	memcpy(ptr, &len, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �� ��Ƽ ���� �ڵ�
	memcpy(ptr, _code, len);
	datasize += len;
	ptr += len;

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_PartyRoom_Leader_Delegate :: ���ο� ��Ƽ ���� ������ ĳ���� �ڵ� : [%s] ", _code);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// ���� ����� ������ġ. (��Ƽ����,�����ڵ�,��ǥ)
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

	// ä�ο� �����ִ� ���� ���� ��ŷ
	partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());
	
	usercount = partyroom_temp->GetPartRoomUserNum();

	// ���� ����
	memcpy(ptr, &usercount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// �˻��ʱ�ȭ
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

		// ��������
		memcpy(ptr, &pos, sizeof(Vector3));
		ptr += sizeof(Vector3);
		size += sizeof(Vector3);

		count++;
	}

	_datasize = size;

	CriticalSectionManager::GetInstance()->Leave();
}

// ���� �������� ����� ������ġ. (��Ƽ����,�����ڵ�,��ǥ)
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

	// ä�ο� �����ִ� ���� ���� ��ŷ
	partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	usercount = partyroom_temp->GetPartRoomUserNum();

	// ���� ����
	memcpy(ptr, &usercount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// �˻��ʱ�ȭ
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

		// ��������
		memcpy(ptr, &pos, sizeof(Vector3));
		ptr += sizeof(Vector3);
		size += sizeof(Vector3);

		count++;
	}

	_datasize = size;

	CriticalSectionManager::GetInstance()->Leave();
}

// ���� �������� ����� ���� ����.
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

	//// ��Ƽ��
	//partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	//// ���� ����
	//memcpy(ptr, &usercount, sizeof(int));
	//ptr += sizeof(int);
	//size += sizeof(int);

	//// �˻��ʱ�ȭ
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

	//	// ��������
	//	memcpy(ptr, &pos, sizeof(Vector3));
	//	ptr += sizeof(Vector3);
	//	size += sizeof(Vector3);

	//	count++;
	//}

	//_datasize = size;

	//CriticalSectionManager::GetInstance()->Leave();
}

// ��Ƽ �ʴ� ��� ��ŷ
void InGameManager::User_Pack_Party_Result(User * _user, bool _result, char * _data, int & _datasize)
{
	int datasize = 0;
	char* ptr = _data;
	char msg[BUFSIZE];
	bool result = _result;

	// ���
	memcpy(ptr, &result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// �޼���
	memset(msg, 0, sizeof(msg));

	if (result)
	{
		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	_datasize = datasize;
}

// ��Ƽ ���� ��� ��ŷ
void InGameManager::User_Pack_Party_Result_Code(User * _user, bool _result, char * _code, char * _data, int & _datasize)
{
	int datasize = 0;

	char* ptr = _data;
	char msg[BUFSIZE];
	int len = strlen(_code);
	bool result = _result;

	// ���
	memcpy(ptr, &result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// �޼���
	memset(msg, 0, sizeof(msg));

	if (result)
	{
		// �ڵ� ����
		memcpy(ptr, &len, sizeof(int));
		datasize += sizeof(int);
		ptr += sizeof(int);

		// �ڵ�
		memcpy(ptr, _code, len);
		datasize += len;
		ptr += len;

		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����] : ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	_datasize = datasize;
}

// ���� ����� ���� ���� ��ŷ
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

	// ��ġ
	memcpy(ptr, &pos, sizeof(Vector3));
	ptr += sizeof(Vector3);
	size += sizeof(Vector3);
	
	_datasize = size;
}

// ä�� �̵� ��û ����
void InGameManager::User_UnPack_ChannelChange(User * _user, char * _buf, int& _channelnum)
{
	int channelnum = 0;

	memcpy(&channelnum, _buf, sizeof(int));

	_channelnum = channelnum;
}

// ĳ���� �ڵ� ���� (�ڵ�)
void InGameManager::User_Unpack_UserCode(User * _user, char * _buf, char * _code)
{
	char* ptr = _buf;
	int len = 0;
	// �ڵ� ����
	memcpy(&len, ptr, sizeof(int));
	ptr += sizeof(int);
	// �ڵ�
	memcpy(_code, ptr, len);
	ptr += len;
}

// �ʴ� ��û ��� ����
void InGameManager::User_Unpack_PartyRoom_Invite_Result(User * _user, char * _buf, bool& _result, char * _code, int & _partyroomnum)
{
	char* ptr = _buf;
	int len = 0;
	bool result = false;
	int partyroomnum;

	// ���
	memcpy(&result, ptr, sizeof(bool));
	ptr += sizeof(bool);
	// �ڵ� ����
	memcpy(&len, ptr, sizeof(int));
	ptr += sizeof(int);
	// �ڵ�
	memcpy(_code, ptr, len);
	ptr += len;
	// ��Ƽ ��ȣ
	memcpy(&partyroomnum, ptr, sizeof(int));
	ptr += sizeof(int);

	_result = result;
	_partyroomnum = partyroomnum;
}

// �̵��̳� ȸ�� ���� Vecotr3. 
void InGameManager::User_Pack_MoveInfoToOther(User* _user, char * _data, int & _datasize)
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

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_MoveInfoToOther :: �ڵ� : [%s] ��ġ: [%f] [%f] [%f]", curcharacter->GetCharacter_Code(), curcharacter->GetPosition().x, curcharacter->GetPosition().y,
		curcharacter->GetPosition().z);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// ���� ĳ���� DB�� ����
void InGameManager::User_CurCharacter_Save(User * _user)
{
	CriticalSectionManager::GetInstance()->Enter();
	char msg[BUFSIZE];

	if (DBManager::GetInstance()->Character_Save(_user->getID(), _user->GetCurCharacter()) == false)
	{
		// �޼���
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "User_CurCharacter_Save :: ����, ID : [%s]", _user->getID());
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		// �޼���
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "User_CurCharacter_Save :: ����, ID : [%s]", _user->getID());
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// �ش�����(�ڵ�ΰ˻�)�� ��Ƽ�� �����ִ°�
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

// ä�� ����
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

// Ư�� ä�� ����
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

// ä�� ������
void InGameManager::User_Leave_Channel(User * _user)
{
	channelsystem->ChannelLeave(_user);
}

// Ư�� ä�� ������
bool InGameManager::User_Leave_Channel(User * _user, int _channelnum)
{
	return channelsystem->ChannelLeave(_user, _channelnum);
}

// ��Ƽ�� ����
bool InGameManager::User_Create_PartyRoom(User * _user)
{
	return m_partysystem->Party_Create(_user);
}

// ��Ƽ�� ����
bool InGameManager::User_Remove_PartyRoom(User * _user)
{
	return m_partysystem->Party_Remove(_user);
}

// ��Ƽ�� ���� �ڵ�
bool InGameManager::User_Remove_PartyRoom(char * _code)
{
	User* user = nullptr;

	user = UserManager::GetInstance()->getUser(_code);

	return m_partysystem->Party_Remove(user);
}

// ��Ƽ�� ���� �߰�(�����ϴ������ڵ�,��Ƽ���ȣ)
bool InGameManager::User_Party_AddUser(User* _user, int _partyroomnum)
{
	return m_partysystem->Partyroom_add_User(_user, _partyroomnum);
}

// �ش� ���� ��Ƽ�� �ο���
int InGameManager::User_PartyRoom_UserNum(User * _user)
{
	return m_partysystem->GetPartyRoomUserNum(_user->GetPartyNum());
}

// ��Ƽ�濡 ȥ���ΰ�
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

// ��Ƽ ���� ����
bool InGameManager::User_PartyRoom_User_Kick(User * _user, char * _code)
{
	return m_partysystem->Party_Kick_User(_user, _code);
}

// ��Ƽ ���� Ż��
bool InGameManager::User_PartRoom_User_Leave(User * _user)
{
	return m_partysystem->Party_User_Leave(_user);;
}

// ��Ƽ�� ����
bool InGameManager::User_PartyRoom_Leader_Delegate(User* _user, char* _code)
{
	return m_partysystem->Party_Leader_Delegate(_user, _code);
}

// ���� ä�� ����
void InGameManager::User_EnterInDun_Channel(User * _user)
{
	PartyRoom* partyroom = nullptr;
	// ��Ƽ �˻�
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// ��Ƽ ä���߰�
	channelsystem->DungeonEnter(partyroom);
}

// ���� ä�� ������
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
		// ���� ä�� ������ ����. �׷� ��Ƽ�������ϴ�.
		return;
	}

	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		// ���� ä�� ����(Ư��ä��)
		if (User_Enter_Channel(user, user->GetChannelNum()) == false)
		{
			// Ư��ä�ο� ���Ӹ��ϸ� �ƹ�ä�ο��� �ִ´�
			if (User_Enter_Channel(_user))
			{
				// ä�� ���� ����. ���������� ���� ���� ����
				User_Pack_Current_ChannelInfo(user, buf, datasize);
				user->Quepack(SERVER_INGAME_DUNGEON_LEAVE_RESULT, buf, datasize);

				User_Pack_PlayerPosData(_user, rdata, rdatasize);
				User_Send_MoveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_CONNECT, rdata, rdatasize);
			}
			else
			{
				// �̺κ� ���� ��������. ���� �ƹ�ä�ο��� ������ ���Ѵٸ� ����ؾ�����

			}
		}
		else
		{
			//ä�� ���� ����. ���������� ���� ���� ����
			User_Pack_Current_ChannelInfo(user, buf, datasize);
			user->Quepack(SERVER_INGAME_DUNGEON_LEAVE_RESULT, buf, datasize);

			User_Pack_PlayerPosData(_user, rdata, rdatasize);
			User_Send_MoveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_CONNECT, rdata, rdatasize);
		}
	}
}

// �ٸ� ���� �̵����� ����(ä�ο� �������ִ� ���������� ����)
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
			// �޼���
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_MoveInfoToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// �ٸ� ���� �ΰ��ӿ��� ���� ���� ����
void InGameManager::User_Send_LeaveInfoToOther(User * _user, PROTOCOL _p, char * _data, int & _datasize)
{
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	char* ptr = buf;
	int datasize = 0;

	User* user = nullptr;

	CriticalSectionManager::GetInstance()->Enter();

	// ä�ο� ���������� ä�ο� ���� �������� ������
	if (_user->isChannel())
	{
		User_Send_Channel_LeaveInfoToOther(_user, _p, _user->GetChannelNum(), _data, _datasize);
	}

	// ��Ƽ�� ���������� ��Ƽ������ �ƴϸ� �׳� ��Ƽ�������� ������
	if (_user->isParty() && _user->isPartyLeader() == false)
	{
		User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_RESULT, _data, _datasize);
	}// ��Ƽ�� ���������� ��Ƽ�����̸� ��Ƽ�� ��Ʈ����.
	else if (_user->isParty() && _user->isPartyLeader())
	{
		User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, buf, datasize);

		// ��Ƽ���ش�
		User_Remove_PartyRoom(_user);
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// ä���̵��ؼ� �������� ������ ä�ο��ִ� ���������� ����
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
			// �޼���
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_Channel_LeaveInfoToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// �ٸ� ���� �ΰ��ӿ��� ���� ���� ä�ο� ����(��Ƽ�� ����)
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
			// �޼���
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// Ư�� ����(code) ��Ƽ �ʴ� ����
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
		// �޼���
		char msg[BUFSIZE];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "User_Send_Party_InviteToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			_p, _datasize, user->GetSendQueueSize());
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// ��Ƽ������ ����
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
			// �޼���
			char msg[BUFSIZE];
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "User_Send_Party_ToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
				_p, _datasize, user->GetSendQueueSize());
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// ������ ������ ä�ο� �����ִ� ���������� ����
void InGameManager::User_Send_Party_Eneter_Dungeon(User * _user, PROTOCOL _p)
{
	PartyRoom* partyroom = nullptr;
	User* user = nullptr;
	char buf[BUFSIZE];
	int datasize = 0;
	memset(buf, 0, sizeof(buf));

	// ������ �����ִ� ��Ƽ���� �˻��ؼ� �ִ´�.
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// ��Ƽ���� �� ������ ä�ο� �����ִ� ���������� �����ٰ� �˷��ش�.
	// ��Ƽ ���� �ڵ� ��ŷ
	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		// ���� �ڵ� ��ŷ
		User_Pack_UserCode(user, buf, datasize);

		// ��Ƽ ������ ���� ä�ο� send
		User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(user, _p, user->GetChannelNum(), buf, datasize);
	}
}

// ������ �������� ä�ο� �����ִ� ���������� ����
void InGameManager::User_Send_Party_Leave_Dungeon(User * _user, PROTOCOL _p)
{
	PartyRoom* partyroom = nullptr;
	User* user = nullptr;
	char buf[BUFSIZE];
	int datasize = 0;
	memset(buf, 0, sizeof(buf));

	// ������ �����ִ� ��Ƽ���� �˻��ؼ� �ִ´�.
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// ��Ƽ���� �� ������ ä�ο� �����ִ� ���������� �����ٰ� �˷��ش�.
	// ��Ƽ ���� �ڵ� ��ŷ
	partyroom->StartSearchPartyRoom();

	while (partyroom->SearchPartyRoom(user))
	{
		// ���� �ڵ� ��ŷ
		User_Pack_UserCode(user, buf, datasize);

		// ��Ƽ ������ ���� ä�ο� send
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

	// ��������
	switch (protocol)
	{
	case CLIENT_INGAME_OTHERPLAYERLIST:		// ��������Ʈ
		User_Pack_OtherUserPosData(_user);
		User_Pack_PlayerPosData(_user, buf, datasize);
		User_Send_MoveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_CONNECT, buf, datasize);
		result = RT_INGAME_OTHERPLAYER_LIST;
		break;
	case CLIENT_INGAME_MOVE_REPORT:			// Ŭ�� �̵�
		User_Pack_Move(_user, buf, datasize, rdata, rdatasize);
		User_Send_MoveInfoToOther(_user, SERVER_INGAME_MOVE_OTHERPLAYERINFO, rdata, rdatasize);
		sendprotocol = SERVER_INGAME_MOVE_RESULT;
		_user->Quepack(sendprotocol, buf, datasize);
		result = RT_INGAME_MOVE;
		break;
	case CLIENT_INGAME_MOVE_ROTATION:		// Ŭ�� ȸ��
		User_Pack_Rotation(_user, buf, datasize);
		User_Send_MoveInfoToOther(_user, SERVER_INGAME_MOVE_ROTATION, buf, datasize);
		_user->SetCallback(false);
		result = RT_INGAME_MOVE;
		break;
	case CLIENT_INGAME_CHANNEL_INFO:		// ä�� ���� ��û
		User_Pack_ChannelInfo(_user);
		result = RT_INGAME_CHANNEL_INFO;
		break;
	case CLIENT_INGAME_CHANNEL_CHANGE:		// ä�� �̵� ��û
		User_UnPack_ChannelChange(_user, buf, choice);
		if (User_Pack_ChannelChangeResult(_user, buf, choice, datasize, oldchannelnum))
		{
			// ä�� �̵� ����
			User_Pack_UserCode(_user, rdata, rdatasize);
			User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, oldchannelnum, rdata, rdatasize);
		}
		sendprotocol = SERVER_INGAME_CHANNLE_CHANGE_RESULT;
		_user->Quepack(sendprotocol, buf, datasize);
		result = RT_INGAME_CHANNEL_CHANGE;
		break;
	case CLIENT_INGAME_MENU_REQ_CHARACTER:	// ĳ���� ����ȭ�� ��û
		User_Pack_UserCode(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);
		User_Leave_Channel(_user);

		sendprotocol = SERVER_INGAME_MENU_RESULT_CHARACTER;
		_user->Quepack(sendprotocol, buf, 0);
		User_CurCharacter_Save(_user);

		// ���� �������� ĳ���� �ʱ�ȭ
		_user->ResetCurCharacter();
		_user->SetLeaveGame();
		_user->SetLeaveDungeon();
		_user->ResetPartyInfo();

		result = RT_INGAME_MENU_CHARACTER;
		break;
	case CLIENT_INGAME_MENU_REQ_LOGOUT:		// �α׾ƿ� ��û
		User_Pack_UserCode(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);
		User_Leave_Channel(_user);

		sendprotocol = SERVER_INGAME_MENU_RESULT_LOGOUT;
		_user->Quepack(sendprotocol, buf, 0);
		User_CurCharacter_Save(_user);

		// ���� ���� �ʱ�ȭ
		_user->ResetUserInfo();
		result = RT_INGAME_MENU_LOGOUT;
		break;
	case CLIENT_INGAME_MENU_EXIT:			// �������� ��û
		// ���� �ȸ���
		result = RT_INGAME_MENU_EXIT;
		break;
	case CLIENT_INGAME_PARTY_ROOM_INVITE:		// ��Ƽ �ʴ� ��û
		memset(charactercode, 0, sizeof(CHARACTERCODESIZE));
		User_Unpack_UserCode(_user, buf, charactercode);

		if (_user->isParty() == false)
		{
			// ��Ƽ �ʴ��� ������ ��Ƽ�� ������ ��Ƽ���� �����.
			User_Create_PartyRoom(_user);
		}

		// ��Ƽ�� ������������
		if (User_IsParty(charactercode) == false)
		{
			User_Pack_Party_InviteToOther(_user, rdata, rdatasize);
			User_Send_Party_InviteToOther(_user, SERVER_INGAME_PARTY_ROOM_INVITE, rdata, rdatasize, charactercode);
		}
		else
		{
			// ��Ƽ�ʴ� ���� ��ŷ�ϰ� �����ش�.
			User_Pack_Party_Result(_user, false, buf, datasize);
			sendprotocol = SERVER_INGAME_PARTY_ROOM_INVITE_RESULT;
			_user->Quepack(sendprotocol, buf, datasize);
			// ������ ��Ƽ�� ����
			User_Remove_PartyRoom(_user);
		}
		result = RT_INGAME_PARTY_INVITE;
		break;
	case CLIENT_INGAME_PARTY_ROOM_ANSWER_INVITE:		// Ŭ�� ��Ƽ �ʴ� ����
		User_Unpack_PartyRoom_Invite_Result(_user, buf, check, charactercode, partyroomnum);
		if (check) // �ʴ� �¶�
		{
			// ��Ƽ ���� �߰�
			check = User_Party_AddUser(_user, partyroomnum);
			if (check)
			{
				// ���� �߰� ����
				User_Pack_Party_CharacterInfoToOther(_user, rdata, rdatasize);
				User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_ADD_USER, rdata, rdatasize);
				sendprotocol = SERVER_INGAME_PARTY_ROOM_ADD_USER;
				_user->Quepack(sendprotocol, rdata, rdatasize);
			}
			else
			{
				// ���� �߰� ����(�ʴ� �޴»������ ���ж�� ������)
				User_Pack_Party_InviteResultToOther(_user, rdata, rdatasize);

				sendprotocol = SERVER_INGAME_PARTY_ROOM_JOIN_RESULT;
				_user->Quepack(sendprotocol, buf, datasize);
			}
		}
		else // �ʴ� ����
		{
			// ��Ƽ �ʴ� ���� ��ŷ�ϰ� �����ش�.
			User_Pack_Party_Result(_user, false, rdata, rdatasize);
			User_Send_Party_InviteToOther(_user, SERVER_INGAME_PARTY_ROOM_INVITE_RESULT, rdata, rdatasize, charactercode);

			// ������ ȥ�ڶ�� �����Ѵ�.
			if (User_PartyRoom_Alone(partyroomnum))
			{
				// ������ ��Ƽ�� ����
				User_Remove_PartyRoom(charactercode);
			}
		}
		result = RT_INGAME_PARTY_INVITE_RESULT;
		break;
	case CLIENT_INGAME_PARTY_USER_KICK:		// ��Ƽ ���� ��û
		User_Unpack_UserCode(_user, buf, charactercode);
		if (_user->isPartyLeader())
		{
			// ���� ����
			if (User_PartyRoom_User_Kick(_user, charactercode))
			{
				// ������� �������� �˷��ֱ�
				User_Pack_Party_Protocol(_user, buf, datasize);
				User_Send_Party_InviteToOther(_user, SERVER_INGAME_PARTY_USER_KICK, buf, datasize, charactercode);

				// ��Ƽ���� ���״� �����ٶ�� �ڵ常 �˷��ֱ�
				User_Pack_Party_KickInfo(_user, rdata, rdatasize, charactercode);
				User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_KICK_INFO, rdata, rdatasize);

				// ��Ƽ����(��������)���Դ� ��Ƽ���� ���п��θ� �˷��ش�. �׸��� ������� ģ�� �ڵ带 �־��ش�.
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
	case CLIENT_INGAME_PARTY_USER_LEAVE:	// ��Ƽ Ż�� ��û
		if (_user->isPartyLeader())
		{
			// ��Ƽ�������� ��Ƽ�� ��Ʈ�ȴٴ� ��Ŷ������
			User_Pack_Party_Protocol(_user, rdata, rdatasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, rdata, rdatasize);

			// ��Ƽ���ش�
			check = User_Remove_PartyRoom(_user);

			// ��������(��Ƽ������������)���� ��Ƽ Ż������ ������
			User_Pack_Party_Result(_user, check, buf, datasize);
			sendprotocol = SERVER_INGAME_PARTY_USER_LEAVE_RESULT;
			_user->Quepack(sendprotocol, buf, datasize);
		}
		else
		{
			// ��Ƽ�������� ��Ƽ�� �Ѹ� Ż���ߴٰ� �˷��ش�.
			User_Pack_UserCode(_user, rdata, rdatasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_INFO, rdata, rdatasize);

			// ��Ƽ���� �Ѹ� �����.
			check = User_PartRoom_User_Leave(_user);

			// ��������(��Ƽ���̿�������)���� ��Ƽ Ż������ ������
			User_Pack_Party_Result(_user, check, buf, datasize);
			sendprotocol = SERVER_INGAME_PARTY_USER_LEAVE_RESULT;
			_user->Quepack(sendprotocol, buf, datasize);

		}
		result = RT_INGAME_PARTY_USER_LEAVE_RESULT;
		break;
	case CLIENT_INGAME_PARTY_LEADER_DELEGATE:		// ��Ƽ�� ����
		User_Unpack_UserCode(_user, buf, charactercode);

		// ��Ƽ�� ����
		if (User_PartyRoom_Leader_Delegate(_user, charactercode))
		{
			// ��Ƽ�� ���� ������ ��Ƽ���� �ٲ������� ��Ƽ�������� ������
			User_Pack_PartyRoom_Leader_Delegate(_user, rdata, rdatasize, charactercode);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_LEADER_DELEGATE, rdata, rdatasize);
			// ���� �������� ��Ƽ�� ���� ����� ������.(�ڵ�+��������)
			User_Pack_Party_Result_Code(_user, true, charactercode, buf, datasize);
			_user->Quepack(SERVER_INGAME_PARTY_LEADER_DELEGATE_RESULT, buf, datasize);
		}
		else
		{
			// ���� �������� ��Ƽ�� ���� ����� ������.(�ڵ�+��������)
			User_Pack_Party_Result_Code(_user, false, charactercode, buf, datasize);
			_user->Quepack(SERVER_INGAME_PARTY_LEADER_DELEGATE_RESULT, buf, datasize);
		}
		result = RT_INGAME_PARTY_LEADER_DELEGATE_RESULT;
		break;
	case CLIENT_INGAME_DUNGEON_ENTER:		// ���� ���� ��û
		User_EnterInDun_Channel(_user);
		User_Send_Party_Eneter_Dungeon(_user, SERVER_INGAME_OTHERPLAYER_LEAVE);
	
		User_Pack_Party_Dungeon_SpawnData(_user, buf, datasize);
		User_Send_Party_ToOther(_user, SERVER_INGAME_DUNGEON_ENTER_RESULT, buf,datasize );
		_user->Quepack(SERVER_INGAME_DUNGEON_ENTER_RESULT, buf, datasize);

		result = RT_INGAME_DUNGEON_ENTER_RESULT;
		break;
	case CLIENT_INGAME_DUNGEON_LEAVE:	// ���� ���� ��û

		User_LeaveInDun_Channel(_user);

		result = RT_INGAME_DUNGEON_LEAVE_RESULT;
		break;
	case CLIENT_INGAME_DUNGEON_STAGE_IN:	// �������� ���� ��������
		// �������� ����� ĳ���� ��ǥ ����
		User_Pack_Party_Dungeon_Stage_SpawnData(_user, buf, datasize);
		User_Send_Party_ToOther(_user, SERVER_INGAME_DUNGEON_STAGE_IN_REULST, buf, datasize);
		_user->Quepack(SERVER_INGAME_DUNGEON_STAGE_IN_REULST, buf, datasize);

		// �������� ����� ���� ���� ����

		User_Send_Party_ToOther(_user, SERVER_INGAME_MONSTER_MOVE_RESULT, buf, datasize);
		_user->Quepack(SERVER_INGAME_MONSTER_MOVE_RESULT, buf, datasize);

		result = RT_INGAME_DUNGEON_STAGE_IN_RESULT;
		break;
	default:
		break;
	}

	return result;
}

// �ΰ��� ���¿��� ���������� ��ó��
bool InGameManager::User_InGame_Compulsion_Exit(User * _user)
{
	char buf[BUFSIZE];
	char* ptr = buf;
	int datasize = 0;
	bool check = false;

	// �޼���
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));


	// ��Ƽ�� �����ְ� ������ ����������
	if (_user->isParty() == true && _user->isDungeon() == true)
	{
		if (_user->isPartyLeader())
		{
			// ��Ƽ�������� ��Ƽ�� ��Ʈ�ȴٴ� ��Ŷ������
			User_Pack_Party_Protocol(_user, buf, datasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, buf, datasize);
			// ��Ƽ���ش�
			User_Remove_PartyRoom(_user);
		}
		else
		{
			// ������ ������ ������ ��ŷ�Ѵ�
			User_Pack_UserCode(_user, buf, datasize);
			// ��Ƽ��, ä�ο� ������ ���������� send�Ѵ�.
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_INFO, buf, datasize);
			User_PartRoom_User_Leave(_user);
		}

		sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� �����ְ� ������ �����ִ� ���¿� ����");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Leave_Channel(_user);

		return true;
	} // ��Ƽ�� �����ְ� ������ ���������ʰ� ä�ο� ����������
	else if (_user->isParty() == true && _user->isChannel() == true && _user->isDungeon() == false)
	{
		if (_user->isPartyLeader())
		{
			// ��Ƽ�������� ��Ƽ�� ��Ʈ�ȴٴ� ��Ŷ������
			User_Pack_Party_Protocol(_user, buf, datasize);
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_ROOM_REMOVE_RESULT, buf, datasize);
			//User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, _user->GetChannelNum(), buf, datasize);

			// ��Ƽ���ش�
			User_Remove_PartyRoom(_user);
		}
		else
		{
			// ������ ������ ������ ��ŷ�Ѵ�
			User_Pack_UserCode(_user, buf, datasize);
			// ��Ƽ��, ä�ο� ������ ���������� send�Ѵ�.
			User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_LEAVE_INFO, buf, datasize);
			User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, _user->GetChannelNum(), buf, datasize);
			User_PartRoom_User_Leave(_user);
		}

		User_Leave_Channel(_user);

		sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� �����ְ� ������ ���������ʰ� ä�ο� �����ִ»��¿� ���� ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	} // ��Ƽ�� ���������ʰ� ä�ο� ����������
	else if (_user->isParty() == false && _user->isChannel() == true)
	{
		User_Pack_UserCode(_user, buf, datasize);
		User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, _user->GetChannelNum(), buf, datasize);
		User_Leave_Channel(_user);

		sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� ���������ʰ� ä�ο� �����ִ»��¿� ����");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	}

	return false;
}
