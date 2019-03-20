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
	// ����, ä�νý��� �ʱ�ȭ
	verification = new CharacterVerification();
	channelsystem = new ChannelSystem();

	channelsystem->InitializeChannel();
	return true;
}

// ���� ����
void InGameManager::EndManager()
{
	delete channelsystem;
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

// �ΰ��ӿ��� ������ ������ ĳ���� �ڵ� ��ŷ
void InGameManager::User_Pack_Leave_InGame(User * _user, char * _data, int & _datasize)
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
	sprintf(msg, "User_Pack_Leave_InGame :: ������ ������ ĳ���� �ڵ� : [%s] ", curcharacter->GetCharacter_Code());
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

// ĳ���� ���� ���� - ���� ������
void InGameManager::User_Unpack_CharacterInfo(User * _user, char * _buf)
{

}

// ä�� �̵� ��û ����
void InGameManager::User_UnPack_ChannelChange(User * _user, char * _buf, int& _channelnum)
{
	int channelnum = 0;

	memcpy(&channelnum, _buf, sizeof(int));

	_channelnum = channelnum;
}

// �ٸ��������� �� �������� ������ ��ŷ
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

// ���� ä�� ������
void InGameManager::User_LeaveInDun_Channel(User * _user)
{
	if (channelsystem->DungeonLeave(_user->GetPartyNum()))
	{

	}
	else
	{

	}

	// ���� ä�� ����
	if (User_Enter_Channel(_user))
	{

	}
	else
	{

	}
}

// ���� ä�� ����
void InGameManager::User_EnterInDun_Channel(User * _user)
{
	// ��Ƽ �˻�

	// ��Ƽ ä���߰�

}

// �ٸ��������� ��������
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
				// �޼���
				char msg[BUFSIZE];
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "User_Send_LeaveInfoToOther :: ������ ����: [%d] �޴� ����: [%d] ���̵�: [%s] ��������: [%d]\n �����ͻ�����: [%d] SendQueue Size: [%d]", _user->getSocket(), user->getSocket(), user->getID(),
					_p, _datasize, user->GetSendQueueSize());
				MsgManager::GetInstance()->DisplayMsg("INFO", msg);
			}
		}

		User_Leave_Channel(_user);
	}
	else
	{
		// ��Ƽ�����ִ� ���������� ����
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
			User_Pack_Leave_InGame(_user, rdata, rdatasize);
			User_Send_Channel_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, oldchannelnum, rdata, rdatasize);
		}

		sendprotocol = SERVER_INGAME_CHANNLE_CHANGE_RESULT;
		_user->Quepack(sendprotocol, buf, datasize);

		result = RT_INGAME_CHANNEL_CHANGE;
		break;
	case CLIENT_INGAME_MENU_REQ_CHARACTER:	// ĳ���� ����ȭ�� ��û
		User_Pack_Leave_InGame(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);

		sendprotocol = SERVER_INGAME_MENU_RESULT_CHARACTER;
		_user->Quepack(sendprotocol, buf, 0);
		User_CurCharacter_Save(_user);

		// ���� �������� ĳ���� �ʱ�ȭ
		_user->ResetCurCharacter();
		_user->SetLeaveGame();

		result = RT_INGAME_MENU_CHARACTER;
		break;
	case CLIENT_INGAME_MENU_REQ_LOGOUT:		// �α׾ƿ� ��û
		User_Pack_Leave_InGame(_user, buf, datasize);
		User_Send_LeaveInfoToOther(_user, SERVER_INGAME_OTHERPLAYER_LEAVE, buf, datasize);

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
	default:
		break;
	}

	return result;
}
