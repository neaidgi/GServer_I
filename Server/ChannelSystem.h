#ifndef _CHANNELSYSTEM_H_
#define _CHANNELSYSTEM_H_

#include "Global.h"
#include "User.h"
#include "PartySystem.h"
#include "MsgManager.h"

#define MAXCHANNELUSER 100
#define MAXCHANNEL 6

class ChannelList 
{
private:
	std::list<User*> TownChannel[MAXCHANNEL];
	std::list<PartyRoom*> InDunChannel;

	std::list<User*>::iterator TownUser;					// 순차 검색용
	std::list<PartyRoom*>::iterator DungeonUser;	
public:
	ChannelList(){}
	~ChannelList(){}

	void StartSearchTownUser(int _channelnum);				// 리스트
	bool SearchTownUser(int _channelnum, User*& _user);		// 리스트
	
	void StartSearchInDunUser();							// 리스트
	bool SearchInDunUser(PartyRoom* _partyuser);			// 리스트

	bool ChannelEnter(User* _user);
	bool ChannelEnter(User* _user, int _channelnum);
	bool ChannelLeave(User* _user);
	bool ChannelLeave(User* _user,int _channelnum);
	bool DungeonEnter(PartyRoom* _partyroom);
	bool DungeonLeave(PartyRoom* _partyroom);

	int GetTownChannelSize(int _channelnum);
};

class ChannelSystem 
{
private:
	ChannelList* channel_list;
public:
	ChannelSystem() { channel_list = nullptr; }
	~ChannelSystem() 
	{
		if (channel_list != nullptr) delete channel_list;
	}

	void InitializeChannel();

	void StartSearchTownUser(int _channelnum);				// 리스트
	bool SearchTownUser(int _channelnum, User*& _user);		// 리스트

	bool ChannelEnter(User* _user);							// 채널입장(빈곳)
	bool ChannelEnter(User* _user,int _channelnum);			// 채널입장(특정한채널)
	void ChannelLeave(User* _user);							// 채널떠나기
	bool ChannelLeave(User* _user, int _channelnum);		// 특정채널떠나기

	void DungeonEnter(PartyRoom* _partyroom);
	bool DungeonLeave(int _partyroomnum);

	int GetChannelSize(int _channelnum);
};
#endif

