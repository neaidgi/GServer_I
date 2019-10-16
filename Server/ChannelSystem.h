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

	void StartSearchTownUser(int _channelnum);				// 채널 리스트 iterator 초기화
	bool SearchTownUser(int _channelnum, User*& _user);		// 채널 리스트 검색
	
	void StartSearchInDunUser();							// 던전 리스트 iterator 초기화
	bool SearchInDunUser(PartyRoom*& _partyuser);			// 던전 리스트

	bool ChannelEnter(User* _user);							// 채널 접속(유저)
	bool ChannelEnter(User* _user, int _channelnum);		// 채널 접속(유저,채널번호)
	bool ChannelLeave(User* _user);							// 채널 퇴장(유저)
	bool ChannelLeave(User* _user,int _channelnum);			// 채널 퇴장(유저,채널번호)
	bool DungeonEnter(PartyRoom* _partyroom);				// 던전 입장
	bool DungeonLeave(PartyRoom* _partyroom);				// 던전 퇴장

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

	void DungeonEnter(PartyRoom* _partyroom);				// 던전 입장
	bool DungeonLeave(int _partyroomnum);					// 던전 퇴장

	int GetChannelSize(int _channelnum);
};
#endif

