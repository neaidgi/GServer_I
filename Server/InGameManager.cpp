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
	m_verification = new CharacterVerification();
	channelsystem = new ChannelSystem();
	m_partysystem = new PartySystem();

	m_verification->Initialize(0,0);
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
	UINT64 sendprotocol = 0;
	char data[BUFSIZE];
	memset(data, 0, sizeof(data));
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

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYERLIST_INFO);
	_user->Quepack(sendprotocol, data, size);

	CriticalSectionManager::GetInstance()->Leave();
}

// ������ ���� ���� ��ŷ
void InGameManager::User_Pack_PlayerPosData(User * _user, char* _data, int& _datasize)
{
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
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "�̵� ������ :: ���̵�: [%s] ��ġ: [%f] [%f] [%f]", _user->getID(), curPos.x, curPos.y,
	//	curPos.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

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

	//// �޼���
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "�̵� �Ϸ� ������ :: ���̵�: [%s] ��ġ: [%f] [%f] [%f]", _user->getID(), curPos.x, curPos.y,
	//	curPos.z, curRot.x, curRot.y, curRot.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

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

	//// �޼���
	//char msg[BUFSIZE];
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "ȸ�� ������ :: ���̵�: [%s] ȸ��: [%f] [%f] [%f]", _user->getID(), curRot.x, curRot.y,
	//	curRot.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

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
	sprintf(msg, "User_Pack_UserCode :: ������ �ڵ� ��ŷ : [%s] ", curcharacter->GetCharacter_Code());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// ä�� ���� ����
void InGameManager::User_Pack_ChannelInfo(User * _user)
{
	UINT64 sendprotocol = 0;
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

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHANNEL, PROTOCOL_CHANNLE_INFO);
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
				character_hp = character_temp->GetCharacter_HP();
				character_mp = character_temp->GetCharacter_MP();

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
void InGameManager::User_Pack_Party_InviteResultToOther(User * _user, char * _data, int & _datasize, bool _result)
{
	int datasize = 0;
	char* ptr = _data;
	char msg[BUFSIZE];
	bool result = false;

	// ���
	memcpy(ptr, &_result, sizeof(bool));
	datasize += sizeof(bool);
	ptr += sizeof(bool);

	// �޼���
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "User_Pack_Party_InviteResultToOther ::  ");
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

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
	char* ptr = _data;
	int size = 0;
	char msg[BUFSIZE];
	PartyRoom* partyroom_temp = nullptr;
	int monstercode = 0;
	int monsternum = 0;
	float time = MONSTERSPAWNTIME;
	Vector3 spawnpos[MONSTER_SPAWNPOS_MAXCOUNT];

	GameDataManager::GetInstance()->Dungeon_Monster_SpawnPos_Vector(spawnpos);

	CriticalSectionManager::GetInstance()->Enter();

	// ��Ƽ��
	partyroom_temp = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	int monster_typecount = partyroom_temp->GetMonsterTypes();

	// ���� ���� ����
	memcpy(ptr, &monster_typecount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// ���� ���� �ð�
	memcpy(ptr, &time, sizeof(float));
	ptr += sizeof(float);
	size += sizeof(float);

	for (int i = 0; i < monster_typecount; i++)
	{
		// ���� �ڵ�
		monstercode = partyroom_temp->GetMonsterCode(i);
		monsternum = FIRSTSTAGE_NORMALMONSTER_1;

		memcpy(ptr, &monstercode, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// ���� ����
		memcpy(ptr, &monsternum, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// ���� ���� ��ǥ
		memcpy(ptr, &spawnpos[i], sizeof(Vector3));
		ptr += sizeof(Vector3);
		size += sizeof(Vector3);

		// �޼���
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "���� ���� ��ǥ :: ���� �ڵ�: [%d] ���� ����: [%d] ��ġ: [%f] [%f] [%f]", monstercode, monsternum, spawnpos[i].x, spawnpos[i].y,
			spawnpos[i].z);
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	_datasize = size;

	CriticalSectionManager::GetInstance()->Leave();
}

// �������� ��ŷ(�����ڵ�,��������)
void InGameManager::User_Pack_AttackNum_Info(User * _user, char * _data, int & _datasize, int _attacknum)
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

	// ���ݹ�ȣ - (�ִϸ��̼ǹ�ȣ)
	memcpy(ptr, &_attacknum, sizeof(int));
	datasize += sizeof(int);
	ptr += sizeof(int);

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "User_Pack_Attack_Info :: ������ �ڵ� ��ŷ : [%s] ������ ���ݹ�ȣ : [%d] ", curcharacter->GetCharacter_Code(),_attacknum);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	_datasize = datasize;
}

// �������� ��ŷ(�����ڵ�,���͹�ȣ,���ݹ�ȣ)
void InGameManager::User_Pack_Monster_AttackNum_Info(User * _user, char * _data, int & _datasize, int _code, int _num, int _attacknum)
{
	char* ptr = _data;
	int size = 0;

	char msg[BUFSIZE];

	// ���� �ڵ�
	memcpy(ptr, &_code, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// ���� ��ȣ
	memcpy(ptr, &_num, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// ���� ������
	memcpy(ptr, &_attacknum, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	_datasize = size;

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "���� ���� ���� ����. �ڵ� : [%d] ��ȣ : [%d] ���ݹ�ȣ: [%d]", _monstercode, _monsternum, _attacknum);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// ���� �ǰݰ��. �ٸ��������� ������ŷ(������,�׾��ٴ��ǹ�)
void InGameManager::User_Pack_MonsterAttackToOher_Result(User * _user, char  *_data, int & _datasize, int _monstercode, int _monsternum, int _damage, bool _isdie)
{
	char* ptr = _data;
	int size = 0;

	char msg[BUFSIZE];

	// ���� �ڵ�
	memcpy(ptr, &_monstercode, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// ���� ��ȣ
	memcpy(ptr, &_monsternum, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// ���� ������
	memcpy(ptr, &_damage, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// �׾�����. ������ true �ƴϸ� false
	memcpy(ptr, &_isdie, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);

	_datasize = size;

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "�ٸ� �������� ���� ���� ����. �ڵ� : [%d] ��ȣ : [%d] ���� ���ط� : [%d]", _monstercode, _monsternum, _damage);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// ���� �ǰݰ��. �ٸ��������� ������ŷ(ĳ�����ڵ�,������,�׾��ٴ��ǹ�)
void InGameManager::User_Pack_User_UnderAttackToOher_Result(User * _user, char * _data, int & _datasize, int _damage, bool _isdie)
{
	char* ptr = _data;
	int size = 0;

	char msg[BUFSIZE];

	int len = strlen(_user->GetCurCharacter()->GetCharacter_Code());

	// ���� �ڵ� ����
	memcpy(ptr, &len, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// ���� �ڵ�
	memcpy(ptr, _user->GetCurCharacter()->GetCharacter_Code(), len);
	ptr += len;
	size += len;

	// ���� ������
	memcpy(ptr, &_damage, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	// �׾�����. ������ true �ƴϸ� false
	memcpy(ptr, &_isdie, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);

	_datasize = size;

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "�ٸ� �������� ���� ���� ����. �ڵ� : [%d] ��ȣ : [%d] ���� ���ط� : [%d]", _monstercode, _monsternum, _damage);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
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
		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����]");
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

		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}
	else
	{
		sprintf(msg, "User_Pack_Party_Result :: ��Ƽ �ʴ� ��� [����]");
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

// ���� �̵����� ��ŷ(���ͼ���,�����ڵ�,���͹�ȣ,������ǥ)
void InGameManager::User_Pack_Monster_MoveInfo(User * _user, int _code, int _num, char* _data, int& _datasize)
{
	char* ptr = _data;
	int size = 0;

	int code = 0;
	int monsternum = 0;

	PartyRoom* partyroom = nullptr;
	MonsterInfo* monsterinfo = nullptr;
	Vector3 pos;

	char msg[BUFSIZE];

	CriticalSectionManager::GetInstance()->Enter();

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	// ���� ���� �˻� ����
	partyroom->StartSearchMonsterinfo();
	while (partyroom->SearchMonsterinfo(monsterinfo))
	{
		if (monsterinfo->GetMonster()->GetMonster_Code() == _code && monsterinfo->GetMonsterNum() == _num)
		{
			code = monsterinfo->GetMonster()->GetMonster_Code();
			monsternum = monsterinfo->GetMonsterNum();

			// ���� �ڵ�
			memcpy(ptr, &code, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);

			// ���� ��ȣ
			memcpy(ptr, &monsternum, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);

			// ���� ��ġ
			memcpy(ptr, &monsterinfo->GetMonster()->GetPosition(), sizeof(Vector3));
			ptr += sizeof(Vector3);
			size += sizeof(Vector3);

			_datasize = size;

			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "���� ���� ����. �ڵ� : [%d] ��ȣ : [%d] x : [%f] y : [%f] z [%f]", code, monsternum, monsterinfo->GetMonster()->GetPosition().x, monsterinfo->GetMonster()->GetPosition().y, monsterinfo->GetMonster()->GetPosition().z);
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

			break;
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// ���� �ǰݰ�� ���� ��ŷ(����������,�����ڵ�,���͹�ȣ,�������ط�,�׾�����) - ���Ͱ� �׾�����
void InGameManager::User_Pack_MonsterAttack_Result(User * _user, bool _result, int _monstercode, int _monsternum, int _damage, bool _isdie)
{
	UINT64 sendprotocol = 0;
	char buf[512];
	memset(buf, 0, sizeof(buf));
	int size = 0;
	char* ptr = buf;

	char msg[BUFSIZE];

	// �ǰ� �����ߴ���
	memcpy(ptr, &_result, sizeof(bool));
	ptr += sizeof(bool);
	size += sizeof(bool);

	if (_result)
	{
		// ���� �ڵ�
		memcpy(ptr, &_monstercode, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// ���� ��ȣ
		memcpy(ptr, &_monsternum, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// ���� ������
		memcpy(ptr, &_damage, sizeof(int));
		ptr += sizeof(int);
		size += sizeof(int);

		// �׾�����. ������ true �ƴϸ� false
		memcpy(ptr, &_isdie, sizeof(bool));
		ptr += sizeof(bool);
		size += sizeof(bool);
	}

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_ATTACK_RESULT);
	_user->Quepack(sendprotocol, buf, size);

	memset(msg, 0, sizeof(msg));
	sprintf(msg, "���� ���� ����. �ڵ� : [%d] ��ȣ : [%d] ���� ���ط� : [%d]", _monstercode, _monsternum, _damage);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// ���� �ǰݰ�� ������ŷ(��������,������,������ false)
void InGameManager::User_Pack_Under_Attack_Result(User * _user, bool _result, int _damage, bool _state)
{
	UINT64 sendprotocol = 0;
	char buf[512];
	memset(buf, 0, sizeof(buf));
	int datasize = 0;
	char* ptr = buf;
	
	// �ǰ��� �ȉ�����
	if (_result == false)
	{
		memcpy(ptr, &_result, sizeof(bool));
		ptr += sizeof(bool);
		datasize += sizeof(bool);
	}
	// ���ݹ޾����� �������
	else if (_result == true)
	{
		memcpy(ptr, &_result, sizeof(bool));
		ptr += sizeof(bool);
		datasize += sizeof(bool);

		memcpy(ptr, &_damage, sizeof(int));
		ptr += sizeof(int);
		datasize += sizeof(int);

		memcpy(ptr, &_result, sizeof(bool));
		ptr += sizeof(bool);
		datasize += sizeof(bool);
	}

	sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_MONSTER_ATTACK_RESULT);
	_user->Quepack(sendprotocol, buf, datasize);
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

// ���� �̵� ���� ����(�����ڵ�,���͹�ȣ,��ǥ)
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

	// ���� �ڵ� ����
	memcpy(&monster_code, ptr, sizeof(int));
	ptr += sizeof(int);

	// ���� ��ȣ ����
	memcpy(&monster_num, ptr, sizeof(int));
	ptr += sizeof(int);

	// ��ġ ����
	memcpy(&curPos, ptr, sizeof(Vector3));
	ptr += sizeof(Vector3);

	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (partyroom->GetMonsterinfo(monster_code, monster_num, monsterinfo) == false)
	{
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "���� �̵� :: ��ǥ �ޱ� ����");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return;
	}

	// ���� ���� ����
	partyroom->SetMonsterinfo(monster_code, monster_num, curPos);

	_code = monster_code;
	_num = monster_num;
	// �޼���
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "���� �̵� :: ���� �ڵ�: [%d]  ���� ��ȣ: [%d] ��ġ: [%f] [%f] [%f]", monster_code, monster_num, curPos.x, curPos.y, curPos.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
}

// ������ Ư�� ���͸� �����ߴٴ� ��Ŷ
void InGameManager::User_Unpack_User_Successfully_Attack_The_Monster(User * _user, char * _buf, int & _monstercode, int & _monsternum, int& _attacknum, Vector3& _dir)
{
	int monster_code = 0;
	int monster_num = 0;
	int attacknum = 0;
	char* ptr = _buf;

	// ���� �ڵ� ����
	memcpy(&monster_code, ptr, sizeof(int));
	ptr += sizeof(int);

	// ���� ��ȣ ����
	memcpy(&monster_num, ptr, sizeof(int));
	ptr += sizeof(int);

	// ���� ��ȣ
	memcpy(&attacknum, ptr, sizeof(int));
	ptr += sizeof(int);

	// ���� ���� x
	memcpy(&_dir.x, ptr, sizeof(float));
	ptr += sizeof(float);

	// ���� ���� y
	memcpy(&_dir.y, ptr, sizeof(float));
	ptr += sizeof(float);

	// ���� ���� z
	memcpy(&_dir.z, ptr, sizeof(float));
	ptr += sizeof(float);

	_monstercode = monster_code;
	_monsternum = monster_num;
	_attacknum = attacknum;
}

// ������ ���������ߴ��� ����(��������)
void InGameManager::User_Unpack_Attack_Info(User * _user, char * _buf, int & _attacknum)
{
	int attacknum = 0;

	memcpy(&attacknum, _buf, sizeof(int));

	_attacknum = attacknum;
}

// ���Ͱ��� ���������ߴ��� ����(��������)
void InGameManager::User_Unpack_Monster_Successfully_Attack_The_User(User * _user, char * _buf, int & _monstercode, int & _monsternum, int & _attacknum, Vector3& _dir)
{
	int monster_code = 0;
	int monster_num = 0;
	int attacknum = 0;
	char* ptr = _buf;

	// ���� �ڵ� ����
	memcpy(&monster_code, ptr, sizeof(int));
	ptr += sizeof(int);

	// ���� ��ȣ ����
	memcpy(&monster_num, ptr, sizeof(int));
	ptr += sizeof(int);

	// ���� ��ȣ
	memcpy(&attacknum, ptr, sizeof(int));
	ptr += sizeof(int);

	// ���ݻ�Ÿ� ���� x
	memcpy(&_dir.x, ptr, sizeof(float));
	ptr += sizeof(float);

	// ���ݻ�Ÿ� ���� y
	memcpy(&_dir.y, ptr, sizeof(float));
	ptr += sizeof(float);

	// ���ݻ�Ÿ� ���� z
	memcpy(&_dir.z, ptr, sizeof(float));
	ptr += sizeof(float);

	_monstercode = monster_code;
	_monsternum = monster_num;
	_attacknum = attacknum;
}

// �̵��̳� ȸ�� ���� Vecotr3. 
void InGameManager::User_Pack_MoveInfoToOther(User* _user, char * _data, int & _datasize)
{
	int datasize = 0;
	int len = strlen(_user->GetCurCharacter()->GetCharacter_Code());
	char* ptr = _data;
	char msg[BUFSIZE];
	int animationnum = 0;
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

	//// �ִϸ��̼� ��ȣ
	//memcpy(ptr, &animationnum, sizeof(int));
	//datasize += sizeof(int);
	//ptr += sizeof(int);

	//// �޼���
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "User_Pack_MoveInfoToOther :: �ڵ� : [%s] ��ġ: [%f] [%f] [%f]", curcharacter->GetCharacter_Code(), curcharacter->GetPosition().x, curcharacter->GetPosition().y,
	//	curcharacter->GetPosition().z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

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
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_LEAVE_RESULT);
				user->Quepack(sendprotocol, buf, datasize);

				sendprotocol = 0;
				User_Pack_PlayerPosData(_user, rdata, rdatasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_CONNECT);
				User_Send_MoveInfoToOther(_user, sendprotocol, rdata, rdatasize);
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
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_LEAVE_RESULT);
			user->Quepack(sendprotocol, buf, datasize);

			sendprotocol = 0;

			User_Pack_PlayerPosData(_user, rdata, rdatasize);
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_CONNECT);
			User_Send_MoveInfoToOther(_user, sendprotocol, rdata, rdatasize);
		}
	}
}

// �������� ��� �� �������� ����
void InGameManager::User_Dungeon_Stage_Rise(User * _user)
{
	PartyRoom* partyroom = nullptr;
	partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());
	if (partyroom == nullptr)
	{
		return;
	}
	partyroom->RiseStage();
	partyroom->SetDungeonMonsterinfo();
}

// �ٸ� ���� �̵����� ����(ä�ο� �������ִ� ���������� ����)
void InGameManager::User_Send_MoveInfoToOther(User* _user, UINT64 _p, char * _data, int & _datasize)
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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_MoveInfoToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// �ٸ� ���� �ΰ��ӿ��� ���� ���� ����
void InGameManager::User_Send_LeaveInfoToOther(User * _user, UINT64 _p, char * _data, int & _datasize)
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
void InGameManager::User_Send_Channel_LeaveInfoToOther(User * _user, UINT64 _p, int _channelnum, char * _data, int & _datasize)
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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_Channel_LeaveInfoToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// �ٸ� ���� �ΰ��ӿ��� ���� ���� ä�ο� ����(��Ƽ�� ����)
void InGameManager::User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members(User * _user, UINT64 _p, int _channelnum, char * _data, int & _datasize)
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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_Channel_LeaveInfoToOther_Exceptions_for_party_members :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// Ư�� ����(code) ��Ƽ �ʴ� ����
void InGameManager::User_Send_Party_InviteToOther(User* _user, UINT64 _p, char* _data, int& _datasize, char* _code)
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
		//char msg[BUFSIZE];
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "User_Send_Party_InviteToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
		//	_p, _datasize, user->GetSendQueueSize());
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	CriticalSectionManager::GetInstance()->Leave();
}

// ��Ƽ������ ���� - ���⼭ ����
void InGameManager::User_Send_Party_ToOther(User * _user, UINT64 _p, char * _data, int & _datasize)
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
			//char msg[BUFSIZE];
			//memset(msg, 0, sizeof(msg));
			//sprintf(msg, "User_Send_Party_ToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
			//	_p, _datasize, user->GetSendQueueSize());
			//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// ������ ������ ä�ο� �����ִ� ���������� ����
void InGameManager::User_Send_Party_Eneter_Dungeon(User * _user, UINT64 _p)
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
void InGameManager::User_Send_Party_Leave_Dungeon(User * _user, UINT64 _p)
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

// ���� �ð� �ʱ�ȭ
void InGameManager::User_PartyRoom_Monster_Time_ReSet(User * _user, int _code, int _num)
{
	PartyRoom* partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	partyroom->Start_MonsterTimer(_code, _num);
}

// �ش� ������ ��Ƽ�� ���� �ð��� ���ص� �ð��� �����°�.
bool InGameManager::User_PartyRoom_Monster_TimeOver_Check(User* _user, int _code, int _num)
{
	PartyRoom* partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

	if (partyroom == nullptr)
	{
		return false;
	}

	return partyroom->End_MonsterTimer(_code, _num);
}


// �ӽ� ������ case���ȿ� ����
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

	char charactercode[CHARACTERCODESIZE];
	memset(charactercode, 0, sizeof(CHARACTERCODESIZE));

	bool check;
	int choice = 0;
	int oldchannelnum = 0;
	int partyroomnum = 0;
	int monstercode = 0;
	int monsternum = 0;
	int attacknum = 0;

	// �α׿�
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
		case PROTOCOL_INGAME_CHARACER:	// �������� �߰�Ʋ ĳ���� ���� �̸�
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_CHARACTER_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_OTHERPLAYERLIST: // �ٸ� ���� ��û
				User_Pack_OtherUserPosData(_user);
				User_Pack_PlayerPosData(_user, buf, datasize);
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_CONNECT);
				User_Send_MoveInfoToOther(_user, othersendprotocol, buf, datasize);
				result = RT_INGAME_OTHERPLAYER_LIST;
				break;
			case PROTOCOL_MOVE_REPORT: // �̵� ����
				User_Pack_Move(_user, buf, datasize, rdata, rdatasize);

				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_OTHERPLAYERINFO);
				//othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_ANIMATION, PROTOCOL_CHARACER_ANIMATION);
				if (_user->isDungeon())
				{
					User_Send_Party_ToOther(_user, othersendprotocol, rdata, rdatasize);
				}
				else
				{
					User_Send_MoveInfoToOther(_user, othersendprotocol, rdata, rdatasize);
				}
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_RESULT);
				_user->Quepack(sendprotocol, buf, datasize);
				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_MOVE_ROTATION: // ȸ�� ����
				User_Pack_Rotation(_user, buf, datasize);
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_ROTATION);
				User_Send_MoveInfoToOther(_user, othersendprotocol, buf, datasize);
				_user->SetCallback(false);
				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_INGAME_MOVE_START_JUMP: // ���� ����
				_user->GetCurCharacter()->SetCharacter_JumpState(true);
				User_Pack_UserCode(_user, buf, datasize);
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_OTHERPLAYER_START_JUMP);
				if (_user->isDungeon())
				{
					User_Send_Party_ToOther(_user, othersendprotocol, buf, datasize);
				}
				else
				{
					User_Send_MoveInfoToOther(_user, othersendprotocol, buf, datasize);
				}

				// �޼���
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "���� ���� : ������ ������ [x] :%f [y] :%f [z] :%f ", _user->GetCurCharacter()->GetPosition().x,
					_user->GetCurCharacter()->GetPosition().y, _user->GetCurCharacter()->GetPosition().z);
				MsgManager::GetInstance()->DisplayMsg("INFO", msg);

				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_INGAME_MOVE_END_JUMP: // ���� ����
				_user->GetCurCharacter()->SetCharacter_JumpState(false);
				//User_Pack_UserCode(_user, buf, datasize);
				//sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_MOVE_OTHERPLAYER_END_JUMP);
				//if (_user->isDungeon())
				//{
				//	User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
				//}
				//else
				//{
				//	User_Send_MoveInfoToOther(_user, sendprotocol, buf, datasize);
				//}
				result = RT_INGAME_MOVE;
				break;
			case PROTOCOL_INGAME_ATTACK: // �����ߴٴ� ����
			{
				int attacknum = 0;
				// ���������� �ߴ��� �˾Ƴ���
				User_Unpack_Attack_Info(_user, buf, attacknum);
				// �� ���������� ��ŷ�ؼ� �����ش�.
				User_Pack_AttackNum_Info(_user, buf, datasize, attacknum);
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_ATTACK);
				if (_user->isDungeon())
				{
					User_Send_Party_ToOther(_user, othersendprotocol, buf, datasize);
				}
				else
				{
					User_Send_MoveInfoToOther(_user, othersendprotocol, buf, datasize);
				}

				// �޼���
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "���� ���� : ������ ������ [x] :%f [y] :%f [z] :%f ", _user->GetCurCharacter()->GetPosition().x,
					_user->GetCurCharacter()->GetPosition().y, _user->GetCurCharacter()->GetPosition().z);
				MsgManager::GetInstance()->DisplayMsg("INFO", msg);

				result = RT_INGAME_ATTACK;
				break;
			}
			case PROTOCOL_INGAME_ATTACK_SUCCESS: // ���� �ǰ����� Ȯ�ο�û
			{
				Vector3 dir;
				PartyRoom* partyroom = nullptr;
				// ���� ������ Ŭ������ �޴´�.
				User_Unpack_User_Successfully_Attack_The_Monster(_user, buf, monstercode, monsternum, attacknum, dir);

				// ���� ĳ���� �� ��ų���� ����
				Character* user_character = _user->GetCurCharacter();
				AttackInfo user_attackinfo = _user->GetCurCharacter()->GetAttackInfo(attacknum);

				// ���� ���� ����.
				MonsterInfo* monster = nullptr;
				partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

				// ��Ƽ���� ������ Ż��
				if (partyroom == nullptr)
				{
					result = RT_INGAME_ATTACK_RESULT;
					break;
				}
				if (_user->isParty() == false)
				{
					result = RT_INGAME_ATTACK_RESULT;
					break;
				}

				// ���Ͱ� ������
				if (partyroom->GetMonsterinfo(monstercode, monsternum, monster) == false)
				{
					// �޼���
					memset(msg, 0, sizeof(msg));
					sprintf(msg, "���� �ǰ� ���� : ���Ͱ� ���������ʽ��ϴ�. [�����ڵ� : %d] [���� ��ȣ : %d] ", monstercode,monsternum);
					MsgManager::GetInstance()->DisplayMsg("INFO", msg);

					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_ATTACK_RESULT);
					User_Pack_MonsterAttack_Result(_user, false, monstercode, monsternum, 0, false);
					_user->Quepack(sendprotocol, buf, datasize);

					result = RT_INGAME_ATTACK_RESULT;
					break;
				}
				// ���Ͱ� �̹� �׾�����
				if (monster->GetMonsterActivate() == false)
				{
					// �޼���
					memset(msg, 0, sizeof(msg));
					sprintf(msg, "���� �ǰ� ���� : ���Ͱ� �̹� �׾����ϴ�. [�����ڵ� : %d] [���� ��ȣ : %d] ", monstercode, monsternum);
					MsgManager::GetInstance()->DisplayMsg("INFO", msg);

					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_ATTACK_RESULT);
					User_Pack_MonsterAttack_Result(_user, false, monstercode, monsternum, 0, false);
					_user->Quepack(sendprotocol, buf, datasize);

					result = RT_INGAME_ATTACK_RESULT;
					break;
				}

				// -- �ǰ����� --
				// ���Ϳ� �Ÿ��� ������
				if (m_verification->AttackVerificate(user_character->GetPosition(),dir,user_attackinfo.attack_range,user_attackinfo.attack_angle,monster->GetMonster()->GetPosition(),monster->GetMonster()->GetMonsterRange()) == false)
				{
					// �޼���
					memset(msg, 0, sizeof(msg));
					sprintf(msg, "���� �ǰ� ���� : ���Ͱ� �ǰݵ����ʾҽ��ϴ�. [�����ڵ� : %d] [���� ��ȣ : %d] ", monstercode, monsternum);
					MsgManager::GetInstance()->DisplayMsg("INFO", msg);

					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_ATTACK_RESULT);
					User_Pack_MonsterAttack_Result(_user, false, monstercode, monsternum, 0, false);
					_user->Quepack(sendprotocol, buf, datasize);

					result = RT_INGAME_ATTACK_RESULT;
					break;
				}

				// -- ������ ó�� -- 
				// �ǰݼ��� �������� �ְ� ü���� 0�̵Ǹ� Ȱ��ȭ�� Ǯ�� �׾��ٰ� �˷�����
				bool islive = partyroom->Monster_HP_Down(monstercode, monsternum, 10);

				User_Pack_MonsterAttack_Result(_user, true, monstercode, monsternum, 10, islive);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_ATTACK_RESULT);
				_user->Quepack(sendprotocol, buf, datasize);

				// �ٸ� ���� ��������
				User_Pack_MonsterAttackToOher_Result(_user, rdata, rdatasize, monstercode, monsternum, 10, islive);
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_OTHERPLAYER_ATTACK_SUCCESS);

				if (_user->isDungeon())
				{
					User_Send_Party_ToOther(_user, othersendprotocol, rdata, rdatasize);
				}
				else
				{
					//User_Send_MoveInfoToOther(_user, othersendprotocol, rdata, rdatasize);
				}
				result = RT_INGAME_ATTACK_RESULT;
				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGMAE_MONSTER: 	// �������� �߰�Ʋ ���� ���� �̸�
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_MONSTER_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_MONSTER_INFO: // ���� ���� ��û
			{
				// �������� ����� ���� ���� ����
				User_Dungeon_Stage_Rise(_user);

				User_Pack_Dungeon_Monster_SpawnInfo(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_INFO);
				User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
				_user->Quepack(sendprotocol, buf, datasize);

				result = RT_INGAME_DUNGEON_MONSTER_INFO_RESULT;
				break;
			}
			case PROTOCOL_MONSTER_MOVE: // ���� �̵�
			{
				User_Unpack_Monster_Move(_user, buf, monstercode, monsternum);

				// �ð��� �������� ���� ������ ������
				if (User_PartyRoom_Monster_TimeOver_Check(_user, monstercode, monsternum))
				{
					User_Pack_Monster_MoveInfo(_user, monstercode, monsternum, rdata, rdatasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_MOVE_RESULT);
					User_Send_Party_ToOther(_user, sendprotocol, rdata, rdatasize);
					_user->Quepack(sendprotocol, rdata, rdatasize);

					// �ð� �ʱ�ȭ
					User_PartyRoom_Monster_Time_ReSet(_user, monstercode, monsternum);
				}

				result = RT_INGMAE_MONSTER_MOVE_RESULT;
				break;
			}
			case PROTOCOL_REQ_MONSTER_ATTACK:	// ���Ͱ� ������ �����ߴ�
			{
				Vector3 dir;
				PartyRoom* partyroom = nullptr;

				bool flag = true;

				monstercode = 0;
				monsternum = 0;
				attacknum = 0;
				// ���� ��������
				User_Unpack_Monster_Successfully_Attack_The_User(_user,buf,monstercode,monsternum,attacknum, dir);

				// ���� ĳ���� �� ��ų���� ����
				Character* user_character = _user->GetCurCharacter();

				// ���� ���� ����.
				MonsterInfo* monster = nullptr;
				MonsterAttackInfo monster_attackinfo;
				partyroom = m_partysystem->GetPartyRoomSearch(_user->GetPartyNum());

				// ��Ƽ���� ������ Ż��
				if (partyroom == nullptr)
				{
					result = RT_INGAME_ATTACK_RESULT;
					break;
				}
				if (_user->isParty() == false)
				{
					result = RT_INGAME_ATTACK_RESULT;
					break;
				}

				// ���Ͱ� ������
				if (partyroom->GetMonsterinfo(monstercode, monsternum, monster) == false)
				{
					// �޼���
					memset(msg, 0, sizeof(msg));
					sprintf(msg, "���� ���� �ǰ� ���� : ���Ͱ� ���������ʽ��ϴ�. [�����ڵ� : %d] [���� ��ȣ : %d] ", monstercode, monsternum);
					MsgManager::GetInstance()->DisplayMsg("INFO", msg);

					User_Pack_Under_Attack_Result(_user, false, 0, flag);

					result = RT_INGAME_ATTACK_RESULT;
					break;
				}

				// ���Ͱ� �̹� �׾�����
				if (monster->GetMonsterActivate() == false)
				{
					// �޼���
					memset(msg, 0, sizeof(msg));
					sprintf(msg, "���� �ǰ� ���� : ���Ͱ� �̹� �׾����ϴ�. [�����ڵ� : %d] [���� ��ȣ : %d] ", monstercode, monsternum);
					MsgManager::GetInstance()->DisplayMsg("INFO", msg);

					User_Pack_Under_Attack_Result(_user, false, 0, flag);

					result = RT_INGAME_ATTACK_RESULT;
					break;
				}

				// ���� ��������
				monster_attackinfo = monster->GetMonster()->GetAttackInfo(attacknum);

				// -- �ǰ����� --
				// ������ �Ÿ��� ������
				if (m_verification->AttackVerificate(monster->GetMonster()->GetPosition(), dir, monster_attackinfo.attack_range, monster_attackinfo.attack_angle, user_character->GetPosition(), user_character->GetCharacterRange()) == false)
				{
					// �޼���
					memset(msg, 0, sizeof(msg));
					sprintf(msg, "���� �ǰ� ���� : ������ �ǰݵ����ʾҽ��ϴ�. [�����ڵ� : %d] [���� ��ȣ : %d] ", monstercode, monsternum);
					MsgManager::GetInstance()->DisplayMsg("INFO", msg);

					User_Pack_Under_Attack_Result(_user, false, 0, flag);

					result = RT_INGAME_ATTACK_RESULT;
					break;
				}

				// -- ������ ó�� -- 
				// �ǰݼ��� �������� �ְ� ü���� 0�̵Ǹ� ....
				flag = _user->CurCharacter_HP_Down(10);
	
				User_Pack_Under_Attack_Result(_user, true, 10, flag);

				// �ٸ� ���� ��Ŷó��
				User_Pack_User_UnderAttackToOher_Result(_user, rdata, rdatasize, 10, flag);
			
				othersendprotocol = _user->BitPackProtocol(othersendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_INGAME_MONSTER_OTHER_ATTACK_RESULT);
			
				if (_user->isDungeon())
				{
					User_Send_Party_ToOther(_user, othersendprotocol, rdata, rdatasize);
				}
				else
				{
					//User_Send_MoveInfoToOther(_user, othersendprotocol, rdata, rdatasize);
				}

				result = RT_INGAME_MONSTER_ATTACK;
				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_CHANNEL:	// �������� �߰�Ʋ ä�� ���� �̸�
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_CHANNEL_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_CHANNEL_INFO: // ä�� ���� ��û
				User_Pack_ChannelInfo(_user);
				result = RT_INGAME_CHANNEL_INFO;
				break;
			case PROTOCOL_REQ_CHANNEL_CHANGE: // ä�� �̵� ��û
				User_UnPack_ChannelChange(_user, buf, choice);
				if (User_Pack_ChannelChangeResult(_user, buf, choice, datasize, oldchannelnum))
				{
					// ä�� �̵� ����
					User_Pack_UserCode(_user, rdata, rdatasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHANNEL, PROTOCOL_CHANNLE_USER_CHANGE);

					User_Send_Channel_LeaveInfoToOther(_user, sendprotocol, oldchannelnum, rdata, rdatasize);
				}
				sendprotocol = 0;
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHANNEL, PROTOCOL_CHANNLE_CHANGE_RESULT);
				_user->Quepack(sendprotocol, buf, datasize);
				result = RT_INGAME_CHANNEL_CHANGE;
				break;
			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_PARTY:		// �������� �߰�Ʋ ��Ƽ ���� �̸�
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_PARTY_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_PARTY_ROOM_INVITE: // ��Ƽ �ʴ��̸�
			{
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
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_INVITE);

					User_Send_Party_InviteToOther(_user, sendprotocol, rdata, rdatasize, charactercode);
				}
				else
				{
					// ��Ƽ�ʴ� ���� ��ŷ�ϰ� �����ش�.
					User_Pack_Party_Result(_user, false, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_INVITE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);
					// ������ ��Ƽ�� ����
					User_Remove_PartyRoom(_user);
				}
				result = RT_INGAME_PARTY_INVITE;
				break;
			}
			case PROTOCOL_PARTY_ROOM_ANSWER_INVITE: // ��Ƽ �ʴ� ����
			{
				User_Unpack_PartyRoom_Invite_Result(_user, buf, check, charactercode, partyroomnum);
				if (check) // �ʴ� �¶�
				{
					// ��Ƽ ���� �߰�
					check = User_Party_AddUser(_user, partyroomnum);
					if (check)
					{
						// ���� �߰� ����
						User_Pack_Party_CharacterInfoToOther(_user, rdata, rdatasize);
						sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_ADD_USER);

						User_Send_Party_ToOther(_user, sendprotocol, rdata, rdatasize);
						_user->Quepack(sendprotocol, rdata, rdatasize);
					}
					else
					{
						// ���� �߰� ����(�ʴ� �޴»������ ���ж�� ������)
						User_Pack_Party_InviteResultToOther(_user, rdata, rdatasize, false);

						sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_JOIN_RESULT);
						_user->Quepack(sendprotocol, buf, datasize);
					}
				}
				else // �ʴ� ����
				{
					// ��Ƽ �ʴ� ���� ��ŷ�ϰ� �����ش�.
					User_Pack_Party_Result(_user, false, rdata, rdatasize);

					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_INVITE_RESULT);

					User_Send_Party_InviteToOther(_user, sendprotocol, rdata, rdatasize, charactercode);

					// ������ ȥ�ڶ�� �����Ѵ�.
					if (User_PartyRoom_Alone(partyroomnum))
					{
						// ������ ��Ƽ�� ����
						User_Remove_PartyRoom(charactercode);
					}
				}
				result = RT_INGAME_PARTY_INVITE_RESULT;
				break;
			}
			case PROTOCOL_REQ_USER_KICK: // ��Ƽ�� ���� ��û
			{
				User_Unpack_UserCode(_user, buf, charactercode);
				if (_user->isPartyLeader())
				{
					// ���� ����
					if (User_PartyRoom_User_Kick(_user, charactercode))
					{
						// ������� �������� �˷��ֱ�
						User_Pack_Party_Protocol(_user, buf, datasize);
						sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_KICK);
						User_Send_Party_InviteToOther(_user, sendprotocol, buf, datasize, charactercode);

						// ��Ƽ���� ���״� �����ٶ�� �ڵ常 �˷��ֱ�
						User_Pack_Party_KickInfo(_user, rdata, rdatasize, charactercode);
						sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_KICK_INFO);
						User_Send_Party_ToOther(_user, SERVER_INGAME_PARTY_USER_KICK_INFO, rdata, rdatasize);

						// ��Ƽ����(��������)���Դ� ��Ƽ���� ���п��θ� �˷��ش�. �׸��� ������� ģ�� �ڵ带 �־��ش�.
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
			case PROTOCOL_REQ_USER_LEAVE: // ��Ƽ Ż�� ��û
			{
				if (_user->isPartyLeader())
				{
					// ��Ƽ�������� ��Ƽ�� ��Ʈ�ȴٴ� ��Ŷ������
					User_Pack_Party_Protocol(_user, rdata, rdatasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_REMOVE_RESULT);
					User_Send_Party_ToOther(_user, sendprotocol, rdata, rdatasize);

					// ��Ƽ���ش�
					check = User_Remove_PartyRoom(_user);

					// ��������(��Ƽ������������)���� ��Ƽ Ż������ ������
					User_Pack_Party_Result(_user, check, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);
				}
				else
				{
					// ��Ƽ�������� ��Ƽ�� �Ѹ� Ż���ߴٰ� �˷��ش�.
					User_Pack_UserCode(_user, rdata, rdatasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_INFO);
					User_Send_Party_ToOther(_user, sendprotocol, rdata, rdatasize);

					// ��Ƽ���� �Ѹ� �����.
					check = User_PartRoom_User_Leave(_user);

					// ��������(��Ƽ���̿�������)���� ��Ƽ Ż������ ������
					User_Pack_Party_Result(_user, check, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);

				}
				result = RT_INGAME_PARTY_USER_LEAVE_RESULT;
				break;
			}
			case PROTOCOL_REQ_LEADER_DELEGATE: // ��Ƽ�� ���� ��û
			{
				User_Unpack_UserCode(_user, buf, charactercode);

				// ��Ƽ�� ����
				if (User_PartyRoom_Leader_Delegate(_user, charactercode))
				{
					// ��Ƽ�� ���� ������ ��Ƽ���� �ٲ������� ��Ƽ�������� ������
					User_Pack_PartyRoom_Leader_Delegate(_user, rdata, rdatasize, charactercode);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_LEADER_DELEGATE);
					User_Send_Party_ToOther(_user, sendprotocol, rdata, rdatasize);
					// ���� �������� ��Ƽ�� ���� ����� ������.(�ڵ�+��������)
					User_Pack_Party_Result_Code(_user, true, charactercode, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_LEADER_DELEGATE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);
				}
				else
				{
					// ���� �������� ��Ƽ�� ���� ����� ������.(�ڵ�+��������)
					User_Pack_Party_Result_Code(_user, false, charactercode, buf, datasize);
					sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_LEADER_DELEGATE_RESULT);
					_user->Quepack(sendprotocol, buf, datasize);
				}
				result = RT_INGAME_PARTY_LEADER_DELEGATE_RESULT;
				break;
			}

			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_DUNGEON:	// �������� �߰�Ʋ ���� ���� �̸�
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_DUNGEON_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_REQ_DUNGEON_ENTER:// ���� ���� ��û
			{
				User_EnterInDun_Channel(_user);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
				User_Send_Party_Eneter_Dungeon(_user, sendprotocol);

				sendprotocol = 0;
				User_Pack_Party_Dungeon_SpawnData(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_ENTER_RESULT);
				User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
				_user->Quepack(sendprotocol, buf, datasize);

				result = RT_INGAME_DUNGEON_ENTER_RESULT;
				break;
			}
			case PROTOCOL_REQ_DUNGEON_LEAVE: // ���� ������ ��û
			{
				User_LeaveInDun_Channel(_user);

				result = RT_INGAME_DUNGEON_LEAVE_RESULT;
				break;
			}
			case PROTOCOL_DUNGEON_STAGE_IN: // �������� ���� ��û
			{
				// �������� ����� ĳ���� ��ǥ ����
				User_Pack_Party_Dungeon_Stage_SpawnData(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_DUNGEON, PROTOCOL_DUNGEON_STAGE_IN_RESULT);

				User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
				_user->Quepack(sendprotocol, buf, datasize);

				// �ӽ÷� ����� �������� ����� ���� ���� �ִ¿�
				User_Dungeon_Stage_Rise(_user);

				sendprotocol = 0;
				User_Pack_Dungeon_Monster_SpawnInfo(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MONSTER, PROTOCOL_MONSTER_INFO);
				User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
				_user->Quepack(sendprotocol, buf, datasize);

				result = RT_INGAME_DUNGEON_STAGE_IN_RESULT;
				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGMAE_MENU:		// �������� �߰�Ʋ �Ŵ� ���� �̸�
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_MENU_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_MENU_REQ_CHARACTER:// ĳ���� ����ȭ������ ��û
			{
				User_Pack_UserCode(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);

				User_Send_LeaveInfoToOther(_user, sendprotocol, buf, datasize);
				User_Leave_Channel(_user);

				sendprotocol = 0;
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MENU, PROTOCOL_MENU_RESULT_CHARACTER);
				_user->Quepack(sendprotocol, buf, 0);
				User_CurCharacter_Save(_user);

				// ���� �������� ĳ���� �ʱ�ȭ
				_user->ResetCurCharacter();
				_user->SetLeaveGame();
				_user->SetLeaveDungeon();
				_user->ResetPartyInfo();

				result = RT_INGAME_MENU_CHARACTER;
				break;
			}
			case PROTOCOL_MENU_REQ_LOGOUT: // �α׾ƿ� ��û
			{
				User_Pack_UserCode(_user, buf, datasize);
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
				User_Send_LeaveInfoToOther(_user, sendprotocol, buf, datasize);
				User_Leave_Channel(_user);

				sendprotocol = 0;
				sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGMAE_MENU, PROTOCOL_MENU_RESULT_LOGOUT);
				_user->Quepack(sendprotocol, buf, 0);
				User_CurCharacter_Save(_user);

				// ���� ���� �ʱ�ȭ
				_user->ResetUserInfo();
				result = RT_INGAME_MENU_LOGOUT;
				break;
			}
			case PROTOCOL_MENU_EXIT:  // ���� ��û
			{
				// ���� �ȸ���
				result = RT_INGAME_MENU_EXIT;
				break;
			}
			default:
				break;
			}
			break;
		case PROTOCOL_INGAME_ANIMATION: // �������� �߰�Ʋ �ִϸ��̼� ���� �̸�
			tempprotocol = protocol & PROTOCOL_CLIENT_INGAME_ANIMATION_COMPART;
			switch (tempprotocol)
			{
			case PROTOCOL_CLIENT_CHARACER_ANIMATION: // ĳ���� �ִϸ��̼�
				break;
			case PROTOCOL_CLIENT_MONSTER_ANIMATION: // ���� �ִϸ��̼�
				break;
			case PROTOCOL_CLIENT_MONSTER_ANIMATION_ATTACK: // ���� ���� �ִϸ��̼�
			{
				monstercode = 0;
				monsternum = 0;
				attacknum = 0;
				// ���������� �ߴ��� �˾Ƴ���
				//User_Unpack_Monster_Attack_Successfully_The_User(_user, buf, monstercode, monsternum, attacknum);
				// �� ���������� ��ŷ�ؼ� �����ش�.
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

		// ������ ���������̸� Ż��
		if (compartrprotocol == PROTOCOL_INGMAE_MENU)
		{
			break;
		}

		// protocol�̶� ���� ��� ����Ʈ ����
		compartrprotocol = compartrprotocol << 1;
	}

	return result;
}

// �ΰ��� ���¿��� ���������� ��ó��
bool InGameManager::User_InGame_Compulsion_Exit(User * _user)
{
	UINT64 sendprotocol = 0;
	UINT64 protocol = 0;
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
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
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_REMOVE_RESULT);
			User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);

			// ��Ƽ���ش�
			User_Remove_PartyRoom(_user);
		}
		else
		{
			// ������ ������ ������ ��ŷ�Ѵ�
			User_Pack_UserCode(_user, buf, datasize);
			// ��Ƽ��, ä�ο� ������ ���������� send�Ѵ�.
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_INFO);
			User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
			User_PartRoom_User_Leave(_user);
		}

		sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� �����ְ� ������ �����ִ� ���¿� ����");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		User_Leave_Channel(_user);

		return true;
	}
	// ��Ƽ�� �����ְ� ������ ���������ʰ� ä�ο� ����������
	else if (_user->isParty() == true && _user->isChannel() == true && _user->isDungeon() == false)
	{
		if (_user->isPartyLeader())
		{
			// ��Ƽ�������� ��Ƽ�� ��Ʈ�ȴٴ� ��Ŷ������
			User_Pack_Party_Protocol(_user, buf, datasize);
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_ROOM_REMOVE_RESULT);
			protocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
			User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
			User_Send_Channel_LeaveInfoToOther(_user, protocol, _user->GetChannelNum(), buf, datasize);

			// ��Ƽ���ش�
			User_Remove_PartyRoom(_user);

			sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� �����ְ� ������ ���������ʰ� ä�ο� �����ִ»��¿� ���� : ������ ");
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}
		else
		{
			// ������ ������ ������ ��ŷ�Ѵ�
			User_Pack_UserCode(_user, buf, datasize);
			// ��Ƽ��, ä�ο� ������ ���������� send�Ѵ�.
			sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_PARTY, PROTOCOL_PARTY_USER_LEAVE_INFO);
			protocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);

			User_Send_Party_ToOther(_user, sendprotocol, buf, datasize);
			User_Send_Channel_LeaveInfoToOther(_user, protocol, _user->GetChannelNum(), buf, datasize);
			User_PartRoom_User_Leave(_user);

			sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� �����ְ� ������ ���������ʰ� ä�ο� �����ִ»��¿� ���� : ��Ƽ���� ");
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		}

		User_Leave_Channel(_user);

		/*sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� �����ְ� ������ ���������ʰ� ä�ο� �����ִ»��¿� ���� ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/
		return true;
	} // ��Ƽ�� ���������ʰ� ä�ο� ����������
	else if (_user->isParty() == false && _user->isChannel() == true)
	{
		User_Pack_UserCode(_user, buf, datasize);
		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_INGAME, PROTOCOL_INGAME_CHARACER, PROTOCOL_INGAME_OTHERPLAYER_LEAVE);
		User_Send_Channel_LeaveInfoToOther(_user, sendprotocol, _user->GetChannelNum(), buf, datasize);
		User_Leave_Channel(_user);

		sprintf(msg, "User_InGame_Compulsion_Exit :: ��Ƽ�� ���������ʰ� ä�ο� �����ִ»��¿� ����");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	}

	return false;
}
