#include "CharacterManager.h"

CharacterManager* CharacterManager::Instance = nullptr;

CharacterManager::CharacterManager()
{
	memset(CharacterOrigin, 0, sizeof(CharacterOrigin));
}

CharacterManager::~CharacterManager()
{
	for (int i = 0; i < MAXCHARACTERORIGIN; i++)
	{
		if (CharacterOrigin[i] != NULL)
		{
			delete CharacterOrigin[i];
		}
	}
}

// 슬롯 데이터 가져오기 index 1 ~ 3 // 슬롯 데이터가 없다면 false 반환
bool CharacterManager::GetCharacter_Slot(User * _user, int _index, SlotData* _slot)
{
	int torigincode;
	int tcode;
	char tjobname[20];
	char tnick[20];
	int tlevel;


	bool result = DBManger::GetInstance()->Character_reqCharacterSlot
	(_user->getID(), _index, &torigincode, tjobname, tnick, &tlevel, &tcode);

	// 슬롯에 캐릭터 없으면 false 반환
	if (result == false)
	{
		_slot = nullptr;
		return result;
	}

	int len;
	len = strlen(tjobname);
	_slot->origincode = torigincode;
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

bool CharacterManager::NickOverlapCheck(User * _user, char * _buf)
{
	int len;
	char nick[NICKNAMESIZE];
	bool check = false;

	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);

	memcpy(nick, _buf, len);

	check = DBManger::GetInstance()->Character_reqCharacterCheckName(nick);

	//_user->pack(SERVER_ID_OVERLAP_CHECK, &check, sizeof(bool));
	//_user->include_wset = true;

	if (check)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CharacterManager::CreateCharacter(User * _user, char* _buf)
{
	int jobcode;
	int len;
	char nick[NICKNAMESIZE];
	bool check = false;

	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);

	memcpy(nick, _buf, len);
	_buf += len;

	memcpy(&jobcode, _buf, sizeof(int));
	_buf += sizeof(int);

	switch (jobcode)
	{
	case TANKER:
		DBManger::GetInstance()->Character_CharacterSlotAdd
		(_user->getID(), _user->GetSlotCount() + 1, CharacterOrigin[0]->GetCharacter_Code(),
			CharacterOrigin[0]->GetCharacter_Name(), nick, 1);

		break;
	case WARRIOR:
		DBManger::GetInstance()->Character_CharacterSlotAdd
		(_user->getID(), _user->GetSlotCount() + 1, CharacterOrigin[1]->GetCharacter_Code(),
			CharacterOrigin[1]->GetCharacter_Name(), nick, 1);
		break;
	case MAGICIAN:
		DBManger::GetInstance()->Character_CharacterSlotAdd
		(_user->getID(), _user->GetSlotCount() + 1, CharacterOrigin[2]->GetCharacter_Code(),
			CharacterOrigin[2]->GetCharacter_Name(), nick, 1);
		break;
	}
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
	for (int i = 0; i < MAXCHARACTERORIGIN; i++)
	{
		CharacterOrigin[i] = new Character();
	}

	// 캐릭터 정보 가져오기
	if (DBManger::GetInstance()->Character_reqCharacterInfo(CharacterOrigin) == false)
	{
		// 로그
		return false;
	}
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
	char* ptr = buf;
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

		bool is_slot;

		if (count == 0)
		{
			is_slot = false;

			// 프로토콜 데이터 패킹
			sendprotocol = SERVER_CHARACTER_SLOT_RESULT;
			memcpy(buf, &is_slot, sizeof(bool));
			_user->pack(sendprotocol, buf, 0);
			_user->include_wset = true;
			result = RT_CHARACTER_SLOTRESULT;
		}
		else
		{
			is_slot = true;
			for (int i = 0; i < count; i++)
			{
				_user->SetSlot(slotdata[i]);
			}
			// 슬롯 로드완료
			_user->SlotLoadComplete();

			// 프로토콜 데이터 패킹
			sendprotocol = SERVER_CHARACTER_SLOT_RESULT;
			memcpy(ptr, &is_slot, sizeof(bool));
			ptr += sizeof(bool);

			memcpy(ptr, &count, sizeof(int));
			ptr += sizeof(bool);

			for (int i = 0; i < count; i++)
			{
				int joblen = strlen(slotdata[i]->jobname);
				int nicklen = strlen(slotdata[i]->nick);
				memcpy(ptr, &joblen, sizeof(int));
				ptr += sizeof(int);

				memcpy(ptr, slotdata[i]->jobname, joblen);
				ptr += joblen;

				memcpy(ptr, &slotdata[i]->level, sizeof(int));
				ptr += sizeof(int);

				memcpy(ptr, &nicklen, sizeof(int));
				ptr += sizeof(int);

				memcpy(ptr, slotdata[i]->nick, nicklen);
				ptr += nicklen;
			}

			_user->pack(sendprotocol, buf, 0);
			_user->include_wset = true;
			result = RT_CHARACTER_SLOTRESULT;
		}
		break;
	case CLIENT_NEW_CHARACTER_MENU:
		sendprotocol = SERVER_CHARACTER_MENU;
		_user->pack(sendprotocol, buf, 0);
		_user->include_wset = true;
		result = RT_CHARACTER_ENTERCREATE;
		break;
	case CLIENT_CHARACTER_ENTER:
		sendprotocol = SERVER_CHARACTER_ENTER_RESULT;
		//_user->pack(sendprotocol, buf, 0);
		//_user->include_wset = true;
		//result = RT_CHARACTER_ENTERGAME;
		break;
	default:
		break;
	}

	return result;
}

RESULT CharacterManager::Character_Management_Process(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char* ptr = buf;
	bool check;
	int choice;

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_REQ_CHARACTER:
		if (NickOverlapCheck(_user, buf))
		{
			// 중복 있음
			check = false;
			result = RT_CHARACTER_NICKOVERLAP_TRUE;
			_user->pack(SERVER_CHARACTER_RESULT, &check, sizeof(bool));
			_user->include_wset = true;
		}
		else
		{
			// 중복 없음
			check = true;
			result = RT_CHARACTER_CREATE;
			CreateCharacter(_user, buf);
			_user->pack(SERVER_CHARACTER_RESULT, &check, sizeof(bool));
			_user->include_wset = true;
		}
		break;
	case CLIENT_CHARACTER_EXIT:				// 캐릭터 생성 취소
		result = RT_CHARACTER_EXIT;
		break;
	default:
		break;
	}

	return result;
}
