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

	std::list<User*>::iterator TownUser;					// ���� �˻���
	std::list<PartyRoom*>::iterator DungeonUser;	
public:
	ChannelList(){}
	~ChannelList(){}

	void StartSearchTownUser(int _channelnum);				// ä�� ����Ʈ iterator �ʱ�ȭ
	bool SearchTownUser(int _channelnum, User*& _user);		// ä�� ����Ʈ �˻�
	
	void StartSearchInDunUser();							// ���� ����Ʈ iterator �ʱ�ȭ
	bool SearchInDunUser(PartyRoom*& _partyuser);			// ���� ����Ʈ

	bool ChannelEnter(User* _user);							// ä�� ����(����)
	bool ChannelEnter(User* _user, int _channelnum);		// ä�� ����(����,ä�ι�ȣ)
	bool ChannelLeave(User* _user);							// ä�� ����(����)
	bool ChannelLeave(User* _user,int _channelnum);			// ä�� ����(����,ä�ι�ȣ)
	bool DungeonEnter(PartyRoom* _partyroom);				// ���� ����
	bool DungeonLeave(PartyRoom* _partyroom);				// ���� ����

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

	void StartSearchTownUser(int _channelnum);				// ����Ʈ
	bool SearchTownUser(int _channelnum, User*& _user);		// ����Ʈ

	bool ChannelEnter(User* _user);							// ä������(���)
	bool ChannelEnter(User* _user,int _channelnum);			// ä������(Ư����ä��)
	void ChannelLeave(User* _user);							// ä�ζ�����
	bool ChannelLeave(User* _user, int _channelnum);		// Ư��ä�ζ�����

	void DungeonEnter(PartyRoom* _partyroom);				// ���� ����
	bool DungeonLeave(int _partyroomnum);					// ���� ����

	int GetChannelSize(int _channelnum);
};
#endif

