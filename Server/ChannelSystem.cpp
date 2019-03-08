#include "ChannelSystem.h"

// 마을채널리스트에서 검색 초기화 (마을채널 번호 필요)
void ChannelList::StartSearchTownUser(int _channelnum)
{
	// 리스트 시작부 세팅
	TownUser = TownChannel[_channelnum].begin();
}

// 마을 채널에서 검색 (마을채널 번호 필요)
bool ChannelList::SearchTownUser(int _channelnum, User* _user)
{
	// 유저 하나씩 꺼내서 save에 넣음
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
// 던전채널리스트 검색 초기화
void ChannelList::StartSearchInDunUser()
{
	DungeonUser = InDunChannel.begin();
}
// 던전채널리스트에서 유저 검색
bool ChannelList::SearchInDunUser(PartyRoom * _partyuser)
{
	// 유저 하나씩 꺼내서 save에 넣음
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
// 채널 입장 
// 자동으로 빈 채널 찾아서 입장
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
// 채널 입장
// 채널번호로 입장 (빈자리 없을때 실패)
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
// 채널 떠나기
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
// 던전 들어감
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
// 던전 나감
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

// 채널 리스트 생성
void ChannelSystem::InitializeChannel()
{
	channel_list = new ChannelList();
}
// 채널 입장 (빈곳 자동 탐색)
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
// 마을 채널 나가기
void ChannelSystem::ChannelLeave(User * _user)
{
	channel_list->ChannelLeave(_user);
}
void ChannelSystem::DungeonEnter(PartyRoom* _partyroom)
{
	channel_list->DungeonEnter(_partyroom);
}
// 던전 채널 나가기
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
