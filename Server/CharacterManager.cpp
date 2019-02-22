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
	int tjobcode;
	char tcode[CHARACTERCODESIZE];
	char tjobname[20];
	char tnick[NICKNAMESIZE];
	int tlevel;

	memset(tcode, 0, sizeof(tcode));
	memset(tjobname, 0, sizeof(tjobname));
	memset(tnick, 0, sizeof(tnick));

	bool result = DBManager::GetInstance()->Character_Req_CharacterSlot
	(_user->getID(), _index, tjobcode, tjobname, tnick, tlevel, tcode);

	char msg[255];
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "슬롯 데이터 : 인덱스 = %d번 직업코드 = %d 직업이름 = %s 닉네임 = %s 레벨 = %d 코드 = %s",
		_index, tjobcode, tjobname, tnick, tlevel, tcode);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// 슬롯에 캐릭터 없으면 false 반환
	if (result == false)
	{
		_slot = nullptr;
		return result;
	}

	_slot->jobcode = tjobcode;
	_slot->level = tlevel;

	int len;
	
	len = strlen(tjobname);
	char* name = new char[len+1];
	memset(name, 0, len+1);
	memcpy(name, tjobname, len);
	name[len] = 0;
	_slot->jobname = name;
	
	len = strlen(tnick);
	_slot->nick = new char[len+1];
	memset(_slot->nick, 0, len+1);
	memcpy(_slot->nick, tnick, len);
	_slot->nick[len] = 0;

	len = strlen(tcode);
	_slot->code = new char[len+1];
	memset(_slot->code, 0, len+1);
	memcpy(_slot->code, tcode, len);
	_slot->code[len] = 0;

	char msg2[BUFSIZE];
	memset(msg2, 0, sizeof(msg2));
	sprintf(msg2, "슬롯 데이터 : 번호 = %d번 직업코드 = %d 직업이름 = %s 닉네임 = %s 레벨 = %d 코드 = %s",
		_index, tjobcode, tjobname, _slot->nick, tlevel, _slot->code);
	LogManager::GetInstance()->LogWrite(msg2);

	return result;
}

// 캐릭터 슬롯 개수
bool CharacterManager::GetCharacter_SlotCount(User * _user, int & _count)
{
	bool result = DBManager::GetInstance()->Character_Req_CharacterSlotCount(_user->getID(), _count);

	if (result)
	{
		return true;
	}
	else
	{
		return false;
	}
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

bool CharacterManager::CreateCharacter(User * _user, char* _buf)
{
	int code;
	int jobcode;
	int len;
	char codebuf[NICKNAMESIZE];
	char nick[NICKNAMESIZE];
	char msg2[BUFSIZE];
	bool check = true;

	memset(msg2, 0, sizeof(msg2));

	// 캐릭터 코드용
	char uniqcode[30];
	memset(uniqcode, 0, sizeof(uniqcode));
	char* codedata = uniqcode;

	//
	memcpy(&len, _buf, sizeof(int));
	_buf += sizeof(int);

	memcpy(nick, _buf, len);
	_buf += len;

	memcpy(&jobcode, _buf, sizeof(int));
	_buf += sizeof(int);

	// 고유코드 만들기 (유저아이디[20] + 캐릭터닉네임[6] + 직업코드[4])
	itoa(jobcode, codebuf, 10);

	memcpy(codedata, _user->getID(), strlen(_user->getID()));
	codedata += strlen(_user->getID());

	// 캐릭터 닉네임길이가 6보다 작으면 길이만큼해서 코드만들기
	if (len >= NICKNAME_CODESIZE)
	{
		memcpy(codedata, nick, NICKNAME_CODESIZE);
		codedata += NICKNAME_CODESIZE;
	}
	else
	{
		memcpy(codedata, nick, len - 1);
		codedata += len - 1;
	}

	memcpy(codedata, codebuf, JOBCODE_SIZE);
	codedata += JOBCODE_SIZE;

	printf("nick %s \n", nick);
	printf("uniqcode %s \n", uniqcode);

	// 설계도
	//const Character * origin = nullptr;

	int count = 0;
	DBManager::GetInstance()->Character_Req_CharacterSlotCount(_user->getID(), count);
	
	// 다음 슬롯
	if (count == SLOTMAXCOUNT)
	{
		check == false;
		sprintf(msg2, "캐릭터 생성 실패 : 슬롯최대수초과\n");
	}
	else
	{
		count += 1;

		switch (jobcode)
		{
		case TANKER:
			DBManager::GetInstance()->Character_CharacterSlotAdd
			(_user->getID(), uniqcode, jobcode, "Tanker", nick, 1, count);
			break;
		case WARRIOR:
			DBManager::GetInstance()->Character_CharacterSlotAdd
			(_user->getID(), uniqcode, jobcode, "Warrior", nick, 1, count);
			break;
		case MAGICIAN:
			DBManager::GetInstance()->Character_CharacterSlotAdd
			(_user->getID(), uniqcode, jobcode, "Magician", nick, 1, count);
			break;
		case GUNNER:
			DBManager::GetInstance()->Character_CharacterSlotAdd
			(_user->getID(), uniqcode, jobcode, "Gunner", nick, 1, count);
			break;
		default:
			check = false;
			sprintf(msg2, "캐릭터 생성 실패 : 직업코드 오류 직업코드 = %d\n", jobcode);
			break;
		}
	}

	if (check)
	{
		sprintf(msg2, "캐릭터 생성 : 번호 = %d번 직업코드 = %d 닉네임 = %s 레벨 = 1 코드 = %s",
			count, jobcode, nick, uniqcode);
		LogManager::GetInstance()->LogWrite(msg2);
	}
	else
	{
		LogManager::GetInstance()->LogWrite(msg2);
	}
	

	// DBManager::GetInstance()->Charactor_CharacterPosAdd(code);
	return check;
}
// 게임시작
void CharacterManager::InitEnterGame(User * _user, char * _buf)
{
	PROTOCOL sendprotocol;
	char data[BUFSIZE];
	char* ptr = data;
	int index = 0;
	int size = 0;
	int len = 0;
	bool enter = true;
	Vector3 pos;
	Vector3 spawnpos[CHARACTER_SPAWNPOS_MAXCOUNT];

	memcpy(&index, _buf, sizeof(int));

	SlotData* slotdata = new SlotData();
	memset(slotdata, 0, sizeof(SlotData));
	GetCharacter_Slot(_user, index, slotdata);
	//DBManager::GetInstance()->Character_Req_CharacterSlot
	//(_user->getID(), index, slotdata.origincode, slotdata.jobname, slotdata.nick, slotdata.level, slotdata.code);

	if (DBManager::GetInstance()->Character_Req_CharacterPos(slotdata->code, pos))
	{
		memcpy(ptr, &enter, sizeof(bool));
		ptr += sizeof(bool);
		size = sizeof(bool);
	}
	else
	{
		// 캐릭터 위치 정보 없을 경우 스폰위치로
		GameDataManager::GetInstance()->Character_SpawnPos_Vector(spawnpos);
		pos = spawnpos[0];

		memcpy(ptr, &enter, sizeof(bool));
		ptr += sizeof(bool);
		size = sizeof(bool);
	}

	Character* player = CharacterSelect(_user, slotdata, index);
	player->SetCharacter_Code(slotdata->code);
	_user->SetCurCharacter(player);
	_user->GetCurCharacter()->SetPosition(pos);

	// 
	// 현재 접속한 캐릭터 패킹
	// 
	char* ptr_temp = ptr;

	// 캐릭터 코드 사이즈
	len = strlen(player->GetCharacter_Code());
	memcpy(ptr_temp, &len, sizeof(int));
	ptr_temp += sizeof(int);
	size += sizeof(int);

	// 캐릭터 코드
	memcpy(ptr_temp, player->GetCharacter_Code(), len);
	ptr_temp += len;
	size += len;

	// 캐릭터 직업코드
	int code = player->GetCharacter_JobCode();
	memcpy(ptr_temp, &code, sizeof(int));
	ptr_temp += sizeof(int);
	size += sizeof(int);
	// 닉네임 사이즈
	len = strlen(player->GetCharacter_Name());
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

// 게임 시작시 실제 캐릭터 조립
Character* CharacterManager::CharacterSelect(User* _user, SlotData*& _slotdata, int _index)
{
	Character* player = new Character();

	// 설계도
	const Character* origin = nullptr;

	// 직업코드에 따라서 설계도 요청
	GameDataManager::GetInstance()->Character_Origin_Data(_slotdata->jobcode, origin);

	//
	//	실제 스테이터스 셋팅 
	//  player
	player->SetCharacter_JobCode(origin->GetCharacter_JobCode());
	player->SetCharacter_JobName(origin->GetCharacter_JobName());
	player->SetCharacter_STR(origin->GetCharacter_STR());
	player->SetCharacter_DEX(origin->GetCharacter_DEX());
	player->SetCharacter_INT(origin->GetCharacter_INT());
	player->SetCharacter_Health(origin->GetCharacter_Health());
	player->SetCharacter_Mana(origin->GetCharacter_Mana());
	player->SetCharacter_AttackPoint(origin->GetCharacter_AttackPoint());
	player->SetCharacter_DefensePoint(origin->GetCharacter_DefensePoint());
	player->SetCharacter_Speed(origin->GetCharacter_Speed());
	player->SetCharacter_Level(_slotdata->level);
	player->SetCharacter_Name(_slotdata->nick);
	player->SetCharacter_Code(_slotdata->code);

	return player;
}

RESULT CharacterManager::Character_Init_Choice(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	char* ptr = buf;
	bool check;
	int choice;

	memset(buf, 0, sizeof(buf));

	_user->unPack(&protocol, &buf);

	PROTOCOL sendprotocol;

	RESULT result;
	SlotData* slotdata[3];
	memset(slotdata, 0, sizeof(slotdata));
	int count = 0;
	int i = 0;
	bool is_slot;
	int size = 0;

	// 수정했음
	switch (protocol)
	{
	case CLIENT_REQ_CHARACTER_SLOT:
		// 슬롯 개수 요청
		if (GetCharacter_SlotCount(_user, count) == false)
		{
			count = 0;
		}

		// DB에서 슬롯캐릭터 받아오기
		while (i < count)
		{
			slotdata[i] = new SlotData();
			memset(slotdata[i], 0, sizeof(SlotData));
			if (GetCharacter_Slot(_user, i + 1, slotdata[i]) == false)
			{
				count = i;
				delete slotdata[i];
				break;
			}

			++i;
		}

		// 슬롯에 캐릭터가 없을경우
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

			//뒤처리 받아온 슬롯데이터 삭제
			for (int i = 0; i < count; i++)
				delete slotdata[i];
		}// else문의 끝
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
	case CLIENT_CHARACTER_DELETE:
		if (DeleateCharacter(_user, buf))
		{
			MsgManager::GetInstance()->DisplayMsg("DB", "캐릭터 삭제 성공");
			check = true;
		}
		else
		{
			MsgManager::GetInstance()->DisplayMsg("DB", "캐릭터 삭제 실패");
			check = false;
		}
		_user->pack(SERVER_CHARACTER_DELETE_RESULT, &check, sizeof(bool));

		result = RT_CHARACTER_DELETE;
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

	switch (protocol)
	{
	case CLIENT_REQ_CHARACTER:
		if (NickOverlapCheck(_user, buf))
		{
			// 중복 있음
			check = false;
			result = RT_CHARACTER_NICKOVERLAP_TRUE;
			_user->pack(SERVER_CHARACTER_RESULT, &check, sizeof(bool));
			//_user->include_wset = true;
		}
		else
		{
			// 중복 없음
			// 캐릭터 개수 제한 체크 추가
			if (CreateCharacter(_user, buf))
			{
				check = true;
				result = RT_CHARACTER_CREATE;
			}
			else
			{
				check = false;
				result = RT_CHARACTER_NICKOVERLAP_TRUE;
			}
			_user->pack(SERVER_CHARACTER_RESULT, &check, sizeof(bool));
			//_user->include_wset = true;
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

// 캐릭터 정보 다른 플레이어에게 전송
//void CharacterManager::CharacterInfo_toOther(User * _user, char * _data, int _datasize)
//{
//	char* ptr = _data;
//	int size = _datasize;
//	const char* name = _user->GetCurCharacter()->GetCharacter_Name();
//	int namesize = strlen(name);
//
//	ptr += size;
//	memcpy(ptr, &namesize, sizeof(int));
//	ptr += sizeof(int);
//	size += sizeof(int);
//
//	memcpy(ptr, name, namesize);
//	size += namesize;
//
//	User* user;
//	while (UserManager::GetInstance()->searchData(user))
//	{
//		if (user->isIngame() && user != _user)
//		{
//			user->pack(SEVER_INGAME_OTHERPLAYER_INFO, _data, _datasize);
//			user->IOCP_OneSided_SendMsg();
//		}
//	}
//}

bool CharacterManager::DeleateCharacter(User * _user, char * _buf)
{
	PROTOCOL sendprotocol;
	int index = 0;
	int size = 0;
	bool enter = true;

	int slot_count = 0;

	memcpy(&index, _buf, sizeof(int));

	MsgManager::GetInstance()->DisplayMsg("DB", "캐릭터 삭제 요청");

	if (DBManager::GetInstance()->Character_Req_CharacterDelete(_user->getID(), index))
	{
		// 스와핑
		if (Character_SlotCount_Calculation(_user,index,slot_count))
		{
			// 뒤에 slot_count만큼 슬롯이더있음
			if (slot_count > 0)
			{
				if (Character_SlotPull(_user, index, slot_count)) 
				{
					// 당기기 성공
					return true;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			// 오류
			return false;
		}
	}
	else
	{
		// 삭제 실패
		return false;
	}

}

bool CharacterManager::Character_SlotCount_Calculation(User* _user,int _index, int& _slotcount)
{
	int slot_count = 0;

	if (DBManager::GetInstance()->Character_Req_SlotCount(_user->getID(), _index, slot_count))
	{
		_slotcount = slot_count;
		return true;
	}
	else
	{
		// 검색실패
		return false;
	}

	
}

bool CharacterManager::Character_SlotPull(User * _user, int _index, int _slotcount)
{
	int slotnum = _index;
	for (int i = 0; i < _slotcount; i++)
	{
		if (DBManager::GetInstance()->Character_Slot_Pull(_user->getID(), slotnum +1, slotnum))
		{
			slotnum++;
		}
		else
		{
			return false;
		}
	}
	return true;
}

//RESULT CharacterManager::Character_EnterGame_Process(User * _user)
//{
//	PROTOCOL protocol;
//	char buf[BUFSIZE];
//	char* ptr = buf;
//	bool check;
//	int datasize = 0;
//
//	_user->unPack(&protocol, &buf);
//
//	RESULT result;
//	PROTOCOL sendprotocol;
//
//	// 수정했음
//	switch (protocol)
//	{
//	case CLIENT_INGAME_MOVE:
//		CharacterMove(_user, buf, datasize);
//		sendprotocol = SEVER_INGAME_MOVE_RESULT;
//		_user->pack(sendprotocol, buf, datasize);
//		result = RT_INGAME_MOVE;
//		CharacterInfo_toOther(_user, buf, datasize);
//	default:
//		break;
//	}
//
//	return result;
//}
