#include "ChannelSystem.h"

// ����ä�θ���Ʈ���� �˻� �ʱ�ȭ (����ä�� ��ȣ �ʿ�)
void ChannelList::StartSearchTownUser(int _channelnum)
{
	// ����Ʈ ���ۺ� ����
	TownUser = TownChannel[_channelnum].begin();
}

// ���� ä�ο��� �˻� (����ä�� ��ȣ �ʿ�)
bool ChannelList::SearchTownUser(int _channelnum, User*& _user)
{
	// ���� �ϳ��� ������ save�� ����
	if (TownUser != TownChannel[_channelnum].end())
	{
		_user = *TownUser;
		++TownUser;
		return true;
	}
	else
	{
		return false;
	}
}

// ����ä�θ���Ʈ �˻� �ʱ�ȭ
void ChannelList::StartSearchInDunUser()
{
	DungeonUser = InDunChannel.begin();
}

// ����ä�θ���Ʈ���� ���� �˻�
bool ChannelList::SearchInDunUser(PartyRoom * _partyuser)
{
	// ���� �ϳ��� ������ save�� ����
	if (DungeonUser != InDunChannel.end())
	{
		_partyuser = *DungeonUser;
		++DungeonUser;
		return true;
	}
	else
	{
		return false;
	}
}

// ä�� ���� 
// �ڵ����� �� ä�� ã�Ƽ� ����
bool ChannelList::ChannelEnter(User* _user)
{
	int channelnum = 0;
	for (int i = 0; i < MAXCHANNEL; i++)
	{
		if (TownChannel[i].size() < MAXCHANNELUSER)
		{
			channelnum = i;
			break;
		}
	}

	if (channelnum >= MAXCHANNEL)
	{
		return false;
	}
	else
	{
		TownChannel[channelnum].push_back(_user);
		_user->SetChannelNum(channelnum);
		return true;
	}
}

// ä�� ����
// ä�ι�ȣ�� ���� (���ڸ� ������ ����)
bool ChannelList::ChannelEnter(User * _user, int _channelnum)
{
	if (TownChannel[_channelnum].size() < MAXCHANNELUSER)
	{
		TownChannel[_channelnum].push_back(_user);
		_user->SetChannelNum(_channelnum);
		return true;
	}
	else
	{
		return false;
	}
}

// ä�� ������
bool ChannelList::ChannelLeave(User * _user)
{
	User* save = nullptr;
	char msg[BUFSIZE];

	if (_user->GetChannelNum() == -1)
	{
		// �޼���
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "ChannelLeave :: ä�ο� ������������");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		return false;
	}

	for (int i = 0; i < MAXCHANNEL; i++)
	{
		StartSearchTownUser(i);
		while (SearchTownUser(i, save))
		{
			if (save == _user)
			{
				TownChannel[i].remove(_user);

				_user->SetChannelNum(-1);
				// �޼���
				memset(msg, 0, sizeof(msg));
				sprintf(msg, "ChannelLeave :: ä�ο��� ������ ���� : [%d �� ä��]", i);
				MsgManager::GetInstance()->DisplayMsg("INFO", msg);

				return true;
			}
		}
	}

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "ChannelLeave :: ä�ο��� ������ ����");
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	return false;
}

// Ư�� ä�� ������
bool ChannelList::ChannelLeave(User * _user, int _channelnum)
{
	User* save = nullptr;
	char msg[BUFSIZE];

	if (_user->GetChannelNum() == -1)
	{
		// �޼���
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "ChannelLeave :: ä�ο� ������������");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);

		return false;
	}


	StartSearchTownUser(_channelnum);
	while (SearchTownUser(_channelnum, save))
	{
		if (save == _user)
		{
			TownChannel[_channelnum].remove(_user);

			// �޼���
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "ChannelLeave(channelnum) :: Ư�� ä�ο��� ������ ���� : [%d �� ä��]", _channelnum);
			MsgManager::GetInstance()->DisplayMsg("INFO", msg);

			return true;
		}
	}

	// �޼���
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "ChannelLeave(channelnum) :: Ư�� ä�ο��� ������ ����");
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	return false;
}

// ���� ��
bool ChannelList::DungeonEnter(PartyRoom* _partyroom)
{
	PartyRoom* save;
	StartSearchInDunUser();
	while (SearchInDunUser(save))
	{
		if (save == _partyroom)
		{
			return false;
		}
	}

	_partyroom->DungeonEnter_PartyRoom();

	InDunChannel.push_back(_partyroom);
	return true;
}

// ���� ����
bool ChannelList::DungeonLeave(PartyRoom * _partyroom)
{
	PartyRoom* save;
	StartSearchInDunUser();
	while (SearchInDunUser(save))
	{
		if (save == _partyroom)
		{
			_partyroom->DungeonLeave_PartyRoom();

			InDunChannel.remove(_partyroom);
			return true;
		}
	}

	return false;
}

// ä�ο� ���� ������
int ChannelList::GetTownChannelSize(int _channelnum)
{
	return TownChannel[_channelnum].size();
}

// ä�� ����Ʈ ����
void ChannelSystem::InitializeChannel()
{
	channel_list = new ChannelList();
}

// ä�� ���� (��� �ڵ� Ž��)
bool ChannelSystem::ChannelEnter(User* _user)
{
	if (channel_list->ChannelEnter(_user))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ä�� ���� (Ư�� ä�� ����)
bool ChannelSystem::ChannelEnter(User* _user, int _channelnum)
{
	if (channel_list->ChannelEnter(_user,_channelnum))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ���� ä�� ������
void ChannelSystem::ChannelLeave(User * _user)
{
	channel_list->ChannelLeave(_user);
}

bool ChannelSystem::ChannelLeave(User * _user, int _channelnum)
{
	return channel_list->ChannelLeave(_user, _channelnum);
}

void ChannelSystem::StartSearchTownUser(int _channelnum)
{
	channel_list->StartSearchTownUser(_channelnum);
}

bool ChannelSystem::SearchTownUser(int _channelnum, User *& _user)
{
	if (channel_list->SearchTownUser(_channelnum, _user))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ChannelSystem::DungeonEnter(PartyRoom* _partyroom)
{
	channel_list->DungeonEnter(_partyroom);
}

// ���� ä�� ������
bool ChannelSystem::DungeonLeave(int _partyroomnum)
{
	channel_list->StartSearchInDunUser();
	PartyRoom* party = nullptr;
	while (channel_list->SearchInDunUser(party))
	{
		if (party->GetPartyRoomNum() == _partyroomnum)
		{
			channel_list->DungeonLeave(party);
			return true;
		}
	}
	return false;
}

int ChannelSystem::GetChannelSize(int _channelnum)
{
	return channel_list->GetTownChannelSize(_channelnum);
}
