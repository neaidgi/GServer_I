#include "CharacterManager.h"

CharacterManager* CharacterManager::Instance = nullptr;

CharacterManager::CharacterManager()
{
	
}

CharacterManager::~CharacterManager()
{
}

// ���� ������ �������� index 1 ~ 3 // ���� �����Ͱ� ���ٸ� false ��ȯ
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
	sprintf(msg, "���� ������ : �ε��� = %d�� �����ڵ� = %d �����̸� = %s �г��� = %s ���� = %d �ڵ� = %s",
		_index, tjobcode, tjobname, tnick, tlevel, tcode);
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// ���Կ� ĳ���� ������ false ��ȯ
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
	sprintf(msg2, "���� ������ : ��ȣ = %d�� �����ڵ� = %d �����̸� = %s �г��� = %s ���� = %d �ڵ� = %s",
		_index, tjobcode, tjobname, _slot->nick, tlevel, _slot->code);
	LogManager::GetInstance()->LogWrite(msg2);

	return result;
}

// ĳ���� ���� ����
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

// ĳ���� �г��� �ߺ�üũ
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

// ĳ���� ����
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

	// ĳ���� �ڵ��
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

	// ���赵
	const Character * origin = nullptr;

	// �ɸ��� �����ڵ� ����
	UUIDManager::GetInstance()->UUID_CharacterUniqCode(_user, codedata, jobcode, len, nick);

	int count = 0;
	DBManager::GetInstance()->Character_Req_CharacterSlotCount(_user->getID(), count);
	
	// ���� ����
	if (count == SLOTMAXCOUNT)
	{
		check == false;
		sprintf(msg2, "ĳ���� ���� ���� : �����ִ���ʰ�\n");
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
			sprintf(msg2, "ĳ���� ���� ���� : �����ڵ� ���� �����ڵ� = %d\n", jobcode);
			break;
		}
	}

	if (check)
	{
		sprintf(msg2, "ĳ���� ���� : ��ȣ = %d�� �����ڵ� = %d �г��� = %s ���� = 1 �ڵ� = %s",
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

// ���ӽ���
bool CharacterManager::InitEnterGame(User * _user, char * _buf)
{
	UINT64 sendprotocol =0;
	char data[BUFSIZE];
	char* ptr = data;
	int index = 0;
	int size = 0;
	int len = 0;
	int spawnposcount = 0;
	bool enter = true;
	Vector3 pos;
	Vector3 rot;
	Vector3 spawnpos[CHARACTER_SPAWNPOS_MAXCOUNT];

	memcpy(&index, _buf, sizeof(int));

	enter = InGameManager::GetInstance()->User_Enter_Channel(_user);

	if (enter)
	{
		SlotData* slotdata = new SlotData();
		memset(slotdata, 0, sizeof(SlotData));
		GetCharacter_Slot(_user, index, slotdata);

		if (DBManager::GetInstance()->Character_Req_CharacterPos(slotdata->code, pos))
		{
			memcpy(ptr, &enter, sizeof(bool));
			ptr += sizeof(bool);
			size = sizeof(bool);
		}
		else
		{
			// ��������
			srand((unsigned int)time(NULL));
			spawnposcount = rand() % (CHARACTER_SPAWNPOS_MAXCOUNT);

			// ĳ���� ��ġ ���� ���� ��� ������ġ��
			GameDataManager::GetInstance()->Character_SpawnPos_Vector(spawnpos);
			pos = spawnpos[spawnposcount];
			
			memcpy(ptr, &enter, sizeof(bool));
			ptr += sizeof(bool);
			size = sizeof(bool);
		}

		Character* player = CharacterSelect(_user, slotdata, index);
		player->SetCharacter_Code(slotdata->code);
		_user->SetCurCharacter(player);
		_user->GetCurCharacter()->SetPosition(pos);

		// �÷��̾ ȸ���� �߰�
		rot.x = 0;
		rot.y = 0;
		rot.z = 0;

		_user->GetCurCharacter()->SetRotation(rot);

		// 
		// ���� ������ ĳ���� ��ŷ
		// 

		int channelnum = _user->GetChannelNum();

		char* ptr_temp = ptr;

		// ĳ���� �ڵ� ������
		len = strlen(player->GetCharacter_Code());
		memcpy(ptr_temp, &len, sizeof(int));
		ptr_temp += sizeof(int);
		size += sizeof(int);

		// ĳ���� �ڵ�
		memcpy(ptr_temp, player->GetCharacter_Code(), len);
		ptr_temp += len;
		size += len;

		// ĳ���� �����ڵ�
		int code = player->GetCharacter_JobCode();
		memcpy(ptr_temp, &code, sizeof(int));
		ptr_temp += sizeof(int);
		size += sizeof(int);
		// �г��� ������
		len = strlen(player->GetCharacter_Name());
		memcpy(ptr_temp, &len, sizeof(int));
		ptr_temp += sizeof(int);
		size += sizeof(int);
		// �г���
		memcpy(ptr_temp, player->GetCharacter_Name(), len);
		ptr_temp += len;
		size += len;
		// ��ġ
		memcpy(ptr_temp, &player->GetPosition(), sizeof(Vector3));
		ptr_temp += sizeof(Vector3);
		size += sizeof(Vector3);
		// ȸ��
		memcpy(ptr_temp, &player->GetRotation(), sizeof(Vector3));
		ptr_temp += sizeof(Vector3);
		size += sizeof(Vector3);
		// ä�� ��ȣ
		memcpy(ptr_temp, &channelnum, sizeof(int));
		ptr_temp += sizeof(int);
		size += sizeof(int);

		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_CHRACTER, PROTOCOL_CHARACER_MENU, PROTOCOL_CHARACTER_ENTER_RESULT);

		_user->Quepack(sendprotocol, data, size);

		delete slotdata;
	}
	else
	{
		memcpy(ptr, &enter, sizeof(bool));
		ptr += sizeof(bool);
		size = sizeof(bool);

		// ä�� ����
		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_CHRACTER, PROTOCOL_CHARACER_MENU, PROTOCOL_CHARACTER_ENTER_RESULT);

		_user->Quepack(sendprotocol, data, size);
	}
	return enter;
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

// ���� ���۽� ���� ĳ���� ����
Character* CharacterManager::CharacterSelect(User* _user, SlotData*& _slotdata, int _index)
{
	Character* player = new Character();

	// ���赵
	const Character* origin = nullptr;

	// �����ڵ忡 ���� ���赵 ��û
	GameDataManager::GetInstance()->Character_Origin_Data(_slotdata->jobcode, origin);

	//
	//	���� �������ͽ� ���� 
	//  player
	player->SetCharacter_JobCode(origin->GetCharacter_JobCode());
	player->SetCharacter_JobName(origin->GetCharacter_JobName());
	player->SetCharacter_STR(origin->GetCharacter_STR());
	player->SetCharacter_DEX(origin->GetCharacter_DEX());
	player->SetCharacter_INT(origin->GetCharacter_INT());
	player->SetCharacter_HP(origin->GetCharacter_HP());
	player->SetCharacter_MP(origin->GetCharacter_MP());
	player->SetCharacter_Current_HP(origin->GetCharacter_HP());
	player->SetCharacter_Current_MP(origin->GetCharacter_MP());
	player->SetCharacter_AttackPoint(origin->GetCharacter_AttackPoint());
	player->SetCharacter_DefensePoint(origin->GetCharacter_DefensePoint());
	player->SetCharacter_Speed(origin->GetCharacter_Speed());
	player->SetCharacter_Level(_slotdata->level);
	player->SetCharacter_Name(_slotdata->nick);
	player->SetCharacter_Code(_slotdata->code);
	player->SetBasicAttack(origin->GetBasicAttack());
	player->SetSecondAttack(origin->GetSecondAttack());
	player->SetScale(origin->GetScale());

	return player;
}

// ĳ���� ����â
RESULT CharacterManager::Character_Management_Process(User * _user)
{
	UINT64 protocol = 0;
	UINT64 sendprotocol = 0;
	UINT64 compartrprotocol = 0;
	UINT64 tempprotocol = 0;
	char buf[BUFSIZE];
	memset(buf, 0, sizeof(buf));
	char* ptr = buf;
	bool check;

	SlotData* slotdata[3];
	memset(slotdata, 0, sizeof(slotdata));
	int count = 0;
	int i = 0;
	bool is_slot;
	int size = 0;

	_user->BitunPack(protocol, &buf);

	RESULT result = RT_DEFAULT;

	compartrprotocol = PROTOCOL_CLIENT_CHARACTER_MENU_COMPART;

	tempprotocol = 0;

	tempprotocol = protocol & compartrprotocol;
	switch (tempprotocol)
	{
	case PROTOCOL_REQ_CHARACTER_CREATE:	// ĳ���� ������û
		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_CHRACTER, PROTOCOL_CHARACER_MENU, PROTOCOL_CHARACTER_RESULT);

		if (NickOverlapCheck(_user, buf))
		{
			// �ߺ� ����
			check = false;
			_user->Quepack(sendprotocol, &check, sizeof(bool));
			result = RT_CHARACTER_NICKOVERLAP_TRUE;
		}
		else
		{
			// �ߺ� ����
			// ĳ���� ���� ���� üũ �߰�
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
			_user->Quepack(sendprotocol, &check, sizeof(bool));
		}
		break;
	case PROTOCOL_CHARACTER_DELETE: // ĳ���� ���� ��û
		if (DeleateCharacter(_user, buf))
		{
			MsgManager::GetInstance()->DisplayMsg("DB", "ĳ���� ���� ����");
			check = true;
		}
		else
		{
			MsgManager::GetInstance()->DisplayMsg("DB", "ĳ���� ���� ����");
			check = false;
		}
		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_CHRACTER, PROTOCOL_CHARACER_MENU, PROTOCOL_CHARACTER_DELETE_RESULT);

		_user->Quepack(sendprotocol, &check, sizeof(bool));

		result = RT_CHARACTER_DELETE;
		break;
	case PROTOCOL_CHARACTER_ENTER:	// ĳ���� ���� ���� ��û
		if (InitEnterGame(_user, buf))
		{
			_user->SetEnterGame();
			result = RT_CHARACTER_ENTERGAME;
		}
		else
		{
			result = RT_CHARACTER_ENTERGAMEFAIL;
		}
		break;
	case PROTOCOL_REQ_CHARACTER_SLOT:	// ���� ���� ��û
		// ���� ���� ��û
		if (GetCharacter_SlotCount(_user, count) == false)
		{
			count = 0;
		}

		// DB���� ����ĳ���� �޾ƿ���
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

		sendprotocol = _user->BitPackProtocol(sendprotocol, PROTOCOL_CHRACTER, PROTOCOL_CHARACER_MENU, PROTOCOL_CHARACTER_SLOT_INFO);

		// ���Կ� ĳ���Ͱ� �������
		if (count == 0)
		{
			is_slot = false;

			memcpy(buf, &is_slot, sizeof(bool));
			_user->Quepack(sendprotocol, buf, sizeof(bool));
			result = RT_CHARACTER_SLOTRESULT;
		}
		else	// ���Կ� ĳ���Ͱ� �������
		{
			is_slot = true;
			size = 0;

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

			_user->Quepack(sendprotocol, buf, size);
			result = RT_CHARACTER_SLOTRESULT;

			//��ó�� �޾ƿ� ���Ե����� ����
			for (int a = 0; a < count; a++)
			{
				delete slotdata[a];
			}
		}
		break;
	default:
		break;
	}
	return result;
}

// ĳ���� ����
bool CharacterManager::DeleateCharacter(User * _user, char * _buf)
{
	int index = 0;
	int size = 0;
	bool enter = true;

	int slot_count = 0;

	memcpy(&index, _buf, sizeof(int));

	MsgManager::GetInstance()->DisplayMsg("DB", "ĳ���� ���� ��û");

	if (DBManager::GetInstance()->Character_Req_CharacterDelete(_user->getID(), index))
	{
		// ������
		if (Character_SlotCount_Calculation(_user,index,slot_count))
		{
			// �ڿ� slot_count��ŭ �����̴�����
			if (slot_count > 0)
			{
				if (Character_SlotPull(_user, index, slot_count)) 
				{
					// ���� ����
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
			// ����
			return false;
		}
	}
	else
	{
		// ���� ����
		return false;
	}

}

// ĳ���� ������ �ڿ� ��ִ��� Ȯ�ο�
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
		// �˻�����
		return false;
	}

	
}

// ĳ���� ���� ����(�����Ƚ��Թ�ȣ,��ܾ���)
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

// ĳ���� ����
bool CharacterManager::Character_Save(User * _user)
{
	return false;
}
