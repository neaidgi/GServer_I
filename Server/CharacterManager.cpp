#include "CharacterManager.h"

CharacterManager* CharacterManager::Instance = nullptr;

CharacterManager::CharacterManager()
{
}

CharacterManager::~CharacterManager()
{
}

// 슬롯 데이터 가져오기 index 1 ~ 3 // 슬롯 데이터가 없다면 false 반환
bool CharacterManager::GetCharacter_Slot(User * _user, int _index, SlotData* _slot)
{
	int tcode;
	char tjobname[20];
	char tnick[20];
	int tlevel;

	bool result = DBManger::GetInstance()->Character_reqCharacterSlot
	(_user->getID(), _index, &tcode, tjobname, tnick, &tlevel);

	// 슬롯에 캐릭터 없으면 false 반환
	if (result == false)
	{
		_slot = nullptr;
		return result;
	}
	
	int len;
	len = strlen(tjobname);
	_slot->code = tcode;
	_slot->level = tlevel;
	_slot->jobname = new char[len + 1]();
	_slot->jobname[len] = '0';
	memcpy(_slot->jobname, tjobname, len);

	len = strlen(tnick);
	_slot->nick = new char[len + 1]();
	_slot->nick[len] = '0';
	memcpy(_slot->nick, tnick, len);

	return result;
}

void CharacterManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new CharacterManager();
	}
}

CharacterManager* CharacterManager::GetInstance()
{
	return Instance;
}

void CharacterManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool CharacterManager::InitializeManager()
{
	
	return true;
}

void CharacterManager::EndManager()
{

}

void CharacterManager::Character_Slot_Send(User * _user)
{

}

RESULT CharacterManager::Character_Init_Choice(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	bool check;
	int choice;

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result;
	SlotData* slotdata[3];
	memset(slotdata, 0, sizeof(slotdata));
	int count = 0;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_REQ_CHARACTER_SLOT:
		for (int i = 0; i < SLOTMAXCOUNT; i++)
		{
			slotdata[i] = new SlotData();
			if (GetCharacter_Slot(_user, i + 1, slotdata[i]) == false)
			{
				delete slotdata[i];
				count = i;
				break;
			}
		}

		if (count == 0)
		{
			bool is_slot = false;
			sendprotocol = SERVER_CHARACTER_SLOT_RESULT;
			memcpy(buf, &is_slot, sizeof(bool));
			_user->pack(sendprotocol, buf, 0);
			_user->include_wset = true;
			result = RT_CHARACTER_SLOTRESULT;
			break;
		}

		for (int i = 0; i < count; i++)
		{
			_user->SetSlot(slotdata[i]);
		}
		_user->SlotLoadComplete();

		//
		//	프로토콜 DATA 패킹 하는 작업
		//
		break;
	case CLIENT_LOGOUT_MENU_CHOICE:

		memcpy(&choice, buf, sizeof(int));

		if (choice == 2)
		{
			sendprotocol = SERVER_JOIN;
			_user->pack(sendprotocol, buf, 0);
			_user->include_wset = true;
			result = RT_JOINMENU;
		}
		else
		{
			result = RT_LOGINMENU;
		}
		break;
	default:
		break;
	}

	return result;
}
