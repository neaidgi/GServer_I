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
	int torigincode;
	int tcode;
	char tjobname[20];
	char tnick[20];
	int tlevel;

	bool result = DBManager::GetInstance()->Character_Req_CharacterSlot
	(_user->getID(), _index, torigincode, tjobname, tnick, tlevel, tcode);

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
	memset(_slot->jobname, 0, len + 1);
	_slot->jobname[len] = '0';
	strcpy_s(_slot->jobname, len + 1, tjobname);
	//memcpy(_slot->jobname, tjobname, len);

	len = strlen(tnick);
	_slot->nick = new char[len + 1]();
	memset(_slot->nick, 0, len + 1);
	_slot->nick[len] = '0';
	strcpy_s(_slot->nick, len + 1, tnick);
	//memcpy(_slot->nick, tnick, len);

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

	check = DBManager::GetInstance()->Character_Req_CharacterCheckName(nick);

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
	int code;
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

	// 고유코드 만들기 (minute + second + millisecond)
	code = CharacterCode();
	
	Character origin[MAXCHARACTERORIGIN];

	for (int i = 0; i < MAXCHARACTERORIGIN; i++)
		GameDataManager::GetInstance()->Character_Origin_Data((i + 1) * 1000, &origin[i]);	// <- 버그

	switch (jobcode)
	{
	case TANKER:
		DBManager::GetInstance()->Character_CharacterSlotAdd
		(_user->getID(), _user->GetSlotCount() + 1, origin[0].GetCharacter_Code(),
			"TANKER", nick, 1, code);

		break;
	case WARRIOR:
		DBManager::GetInstance()->Character_CharacterSlotAdd
		(_user->getID(), _user->GetSlotCount() + 1, origin[1].GetCharacter_Code(),
			"WARRIOR", nick, 1, code);
		break;
	case MAGICIAN:
		DBManager::GetInstance()->Character_CharacterSlotAdd
		(_user->getID(), _user->GetSlotCount() + 1, origin[2].GetCharacter_Code(),
			"MAGICIAN", nick, 1, code);
		break;

	case GUNNER:
		DBManager::GetInstance()->Character_CharacterSlotAdd
		(_user->getID(), _user->GetSlotCount() + 1, origin[3].GetCharacter_Code(),
			"GUNNER", nick, 1, code);
		break;
	}

	DBManager::GetInstance()->Charactor_CharacterPosAdd(code);
}

void CharacterManager::InitEnterGame(User * _user, char * _buf)
{
	PROTOCOL sendprotocol;
	char data[BUFSIZE];
	char* ptr = data;
	int index = 0;
	int size = 0;
	bool enter = true;
	Vector3 pos;
	Vector3 spawnpos[CHARACTER_SPAWNPOS_MAXCOUNT];

	memcpy(&index, _buf, sizeof(int));

	if (DBManager::GetInstance()->Character_Req_CharacterPos(_user->GetSlot(index)->code, pos))
	{
		memcpy(ptr, &enter, sizeof(bool));
		ptr += sizeof(bool);
		size = sizeof(bool);

		memcpy(ptr, &pos.x, sizeof(float));
		ptr += sizeof(float);
		size = sizeof(float);

		memcpy(ptr, &pos.y, sizeof(float));
		ptr += sizeof(float);
		size = sizeof(float);

		memcpy(ptr, &pos.z, sizeof(float));
		ptr += sizeof(float);
		size = sizeof(float);
	}
	else
	{
		// 캐릭터 위치 정보 없을 경우 스폰위치로
		GameDataManager::GetInstance()->Character_SpawnPos_Vector(spawnpos);
		pos = spawnpos[0];

		memcpy(ptr, &enter, sizeof(bool));
		ptr += sizeof(bool);
		size = sizeof(bool);

		memcpy(ptr, &pos.x, sizeof(float));
		ptr += sizeof(float);
		size = sizeof(float);

		memcpy(ptr, &pos.y, sizeof(float));
		ptr += sizeof(float);
		size = sizeof(float);

		memcpy(ptr, &pos.z, sizeof(float));
		ptr += sizeof(float);
		size = sizeof(float);
	}

	Character* player = CharacterSelect(_user, index);
	player->SetCharacter_UniqueCode(_user->GetSlot(index)->code);
	_user->SetCurCharacter(player);
	_user->GetCurCharacter()->SetPosition(pos);

	// 
	// 현재 접속한 캐릭터 패킹
	// 
	char* ptr_temp = ptr;

	// 캐릭터 코드
	int code = player->GetCharacter_Code();
	memcpy(ptr_temp, &code, sizeof(int));
	ptr_temp += sizeof(int);
	size += sizeof(int);
	// 닉네임 사이즈
	int len = strlen(player->GetCharacter_Name());
	memcpy(ptr_temp, &len, sizeof(int));
	ptr_temp += sizeof(int);
	size += sizeof(int);
	// 닉네임
	memcpy(ptr_temp, player->GetCharacter_Name(), len);
	ptr_temp += len;
	size += len;
	// 위치
	memcpy(ptr_temp, &player->GetPosition(), sizeof(Vector3));
	ptr_temp += sizeof(Vector3);
	size += sizeof(Vector3);

	sendprotocol = SERVER_CHARACTER_ENTER_RESULT;
	_user->pack(sendprotocol, data, size);

	_user->SetEnterGame();
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

Character* CharacterManager::CharacterSelect(User* _user, int _index)
{
	Character temp;
	GameDataManager::GetInstance()->Character_Origin_Data(_user->GetSlot(_index)->origincode, &temp);

	Character* player = new Character();

	//
	//	실제 스테이터스 셋팅 
	//  player

	*player = temp;

	//DBManager::GetInstance()->Character_Req_CharacterName(_user->getID(), _user->GetSlot(_index)->)

	player->SetCharacter_Name(_user->GetSlot(_index)->jobname);

	return player;
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
	int i = 0;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_REQ_CHARACTER_SLOT:
		while (i < SLOTMAXCOUNT)
		{
			slotdata[i] = new SlotData();
			if (GetCharacter_Slot(_user, i + 1, slotdata[i]) == false)
			{
				delete slotdata[i];
				count = i;
				break;
			}

			++i;
			count = i;
		}

		bool is_slot;
		int size;

		if (count == 0)
		{
			is_slot = false;

			// 프로토콜 데이터 패킹
			sendprotocol = SERVER_CHARACTER_SLOT_RESULT;
			memcpy(buf, &is_slot, sizeof(bool));
			_user->pack(sendprotocol, buf, sizeof(bool));
			result = RT_CHARACTER_SLOTRESULT;
		}
		else
		{
			is_slot = true;
			size = 0;

			// **슬롯 저장 안하게 수정 예정**
			for (int i = 0; i < count; i++)
			{
				_user->SetSlot(slotdata[i]);
			}
			// 슬롯 로드완료
			_user->SlotLoadComplete();
			// ****

			// 프로토콜 데이터 패킹
			sendprotocol = SERVER_CHARACTER_SLOT_RESULT;

			memcpy(ptr, &is_slot, sizeof(bool));
			ptr += sizeof(bool);
			size += sizeof(bool);

			memcpy(ptr, &count, sizeof(int));
			ptr += sizeof(int);
			size += sizeof(int);

			for (int i = 0; i < count; i++)
			{
				int joblen = strlen(slotdata[i]->jobname);
				int nicklen = strlen(slotdata[i]->nick);

				memcpy(ptr, &joblen, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);

				memcpy(ptr, slotdata[i]->jobname, joblen);
				ptr += joblen;
				size += joblen;

				memcpy(ptr, &slotdata[i]->level, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);

				memcpy(ptr, &nicklen, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);

				memcpy(ptr, slotdata[i]->nick, nicklen);
				ptr += nicklen;
				size += nicklen;

				memcpy(ptr, &slotdata[i]->code, sizeof(int));
				ptr += sizeof(int);
				size += sizeof(int);
			}

			_user->pack(sendprotocol, buf, size);
			result = RT_CHARACTER_SLOTRESULT;
		}
		break;
	case CLIENT_NEW_CHARACTER_MENU:
		sendprotocol = SERVER_CHARACTER_MENU;
		_user->pack(sendprotocol, buf, 0);
		result = RT_CHARACTER_ENTERCREATE;
		break;
	case CLIENT_CHARACTER_ENTER:
		InitEnterGame(_user, buf);
		_user->SetEnterGame();
		result = RT_CHARACTER_ENTERGAME;
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
		_user->pack(SERVER_CHARACTER_EXIT_RESULT, buf, 0);
		result = RT_CHARACTER_EXIT;
		break;
	default:
		break;
	}

	return result;
}

void CharacterManager::CharacterMove(User * _user, char * _buf, int & _datasize)
{
	Vector3 dirvector;
	int datasize;
	float time = MOVETIME;
	char* ptr = _buf;

	memcpy(&dirvector.x, ptr, sizeof(float));
	ptr += sizeof(float);

	memcpy(&dirvector.y, ptr, sizeof(float));
	ptr += sizeof(float);

	memcpy(&dirvector.z, ptr, sizeof(float));
	ptr += sizeof(float);

	memset(_buf, 0, sizeof(_buf));

	ptr = _buf;

	Vector3 character_pos;
	character_pos = _user->GetCurCharacter()->GetPosition();
	character_pos = character_pos + (dirvector * _user->GetCurCharacter()->GetCharacter_Speed());

	_user->GetCurCharacter()->SetPosition(character_pos);

	memcpy(ptr, &character_pos.x, sizeof(float));
	ptr += sizeof(float);
	datasize += sizeof(float);

	memcpy(ptr, &character_pos.y, sizeof(float));
	ptr += sizeof(float);
	datasize += sizeof(float);

	memcpy(ptr, &character_pos.z, sizeof(float));
	ptr += sizeof(float);
	datasize += sizeof(float);

	memcpy(ptr, &time, sizeof(float));
	ptr += sizeof(float);
	datasize += sizeof(float);

	_datasize = datasize;
}

// 캐릭터 정보 다른 플레이어에게 전송
void CharacterManager::CharacterInfo_toOther(User * _user, char * _data, int _datasize)
{
	char* ptr = _data;
	int size = _datasize;
	const char* name = _user->GetCurCharacter()->GetCharacter_Name();
	int namesize = strlen(name);

	ptr += size;
	memcpy(ptr, &namesize, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	memcpy(ptr, name, namesize);
	size += namesize;

	User* user;
	while (UserManager::GetInstance()->searchData(user))
	{
		if (user->isIngame() && user != _user)
		{
			user->pack(SEVER_INGAME_OTHERPLAYER_INFO, _data, _datasize);
			user->IOCP_OneSided_SendMsg();
		}
	}
}

int CharacterManager::CharacterCode()
{
	// 임시사용 수정할 예정

	SYSTEMTIME lptime;

	GetLocalTime(&lptime);

	int code = 0;

	int minute = 0;
	int second = 0;
	int millisecond = 0;

	minute = (lptime.wMinute * 1000000);
	second = (lptime.wSecond * 10000);
	millisecond = (lptime.wMilliseconds * 1);

	code = minute + second + millisecond;

	return code;
}

RESULT CharacterManager::Character_EnterGame_Process(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char* ptr = buf;
	bool check;
	int datasize = 0;

	_user->unPack(&protocol, &buf);

	RESULT result;
	PROTOCOL sendprotocol;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_INGAME_MOVE:
		CharacterMove(_user, buf, datasize);
		sendprotocol = SEVER_INGAME_MOVE_RESULT;
		_user->pack(sendprotocol, buf, datasize);
		result = RT_INGAME_MOVE;
		CharacterInfo_toOther(_user, buf, datasize);
	default:
		break;
	}

	return result;
}
