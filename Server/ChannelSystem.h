#ifndef _CHANNELSYSTEM_H_
#define _CHANNELSYSTEM_H_

#include "Global.h"
#include "User.h"
#include "PartySystem.h"

#define MAXCHANNELUSER 100

class ChannelList {
private:
	std::list<User*> TownChannel[5];
	std::list<PartyRoom*> InDunChannel;

	std::list<User*>::iterator TownUser;			// ���� �˻���
	std::list<PartyRoom*>::iterator DungeonUser;	
public:
	ChannelList(){}
	~ChannelList(){}

	void StartSearchTownUser(int _channelnum);				// ����Ʈ
	bool SearchTownUser(int _channelnum, User* _user);		// ����Ʈ
	
	void StartSearchInDunUser();				// ����Ʈ
	bool SearchInDunUser(PartyRoom* _partyuser);		// ����Ʈ

	bool ChannelEnter(User* _user);
	bool ChannelEnter(int _channelnum, User* _user);
	bool ChannelLeave(User* _user);
	bool DungeonEnter(PartyRoom* _partyroom);
	bool DungeonLeave(PartyRoom* _partyroom);
};

class ChannelSystem {
private:
	ChannelList* channel_list;
public:
	ChannelSystem() { channel_list = nullptr; }
	~ChannelSystem() 
	{
		if (channel_list != nullptr) delete channel_list;
	}

	void InitializeChannel();
};
#endif

