#include "ChannelSystem.h"

// ����ä�θ���Ʈ���� �˻� �ʱ�ȭ (����ä�� ��ȣ �ʿ�)
void ChannelList::StartSearchTownUser(int _channelnum)
{
	// ����Ʈ ���ۺ� ����
	TownUser = TownChannel[_channelnum].begin();
}

// ���� ä�ο��� �˻� (����ä�� ��ȣ �ʿ�)
bool ChannelList::SearchTownUser(int _channelnum, User* _user)
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
	for (int i = 0; i<5; i++)
	{
		if (TownChannel[i].size() < MAXCHANNELUSER)
		{
			channelnum = i;
			break;
		}
	}

	if (channelnum == 0)
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
bool ChannelList::ChannelEnter(int _channelnum, User * _user)
{
	if (TownChannel[_channelnum].size() > MAXCHANNELUSER)
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
	for (int i = 0; i < 5; i++)
	{
		StartSearchTownUser(i);
		while (SearchTownUser(i, save))
		{
			if (save == _user)
			{
				TownChannel[i].remove(_user);
				return true;
			}
		}
	}

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
			InDunChannel.remove(_partyroom);
			return true;
		}
	}

	return false;
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
// ���� ä�� ������
void ChannelSystem::ChannelLeave(User * _user)
{
	channel_list->ChannelLeave(_user);
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
		if (party->partyroom_num == _partyroomnum)
		{
			channel_list->DungeonLeave(party);
			return true;
		}
	}

	return false;
}
