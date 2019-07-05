#include"PartySystem.h"

// Class PartyRoom

PartyRoom::PartyRoom(int _partyroomnum)
{
	m_monster_control = new MonsterControl();
	partyroom_num = _partyroomnum;
	m_dungeon_stage_num = DEFAULT_STAGE;
}

PartyRoom::~PartyRoom()
{
	User* target = nullptr;
	char msg[BUFSIZE];
	for (save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		m_partyuser.remove(target);

		// 메세지
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "~PartyRoom :: 파티방 소멸자 ");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	}

	if (m_monster_control != nullptr)
	{
		delete m_monster_control;
	}
}

// 초기화
void PartyRoom::InitPartyRoom()
{

}

// 마무리
void PartyRoom::DestroyPartyRoom()
{

}

// 검색 초기화
void PartyRoom::StartSearchPartyRoom()
{
	save = m_partyuser.begin();
}

// 파티원(유저) 전달
bool PartyRoom::SearchPartyRoom(User *& _user)
{
	// 유저 하나씩 꺼내서 save에 넣음
	if (save != m_partyuser.end())
	{
		_user = *save;
		++save;
		return true;
	}
	else
	{
		return false;
	}
}

// 파티방 번호가 무엇인가
int PartyRoom::GetPartyRoomNum()
{
	return partyroom_num;
}

// 현재 파티원 인원수
int PartyRoom::GetPartRoomUserNum()
{
	return m_partyuser.size();
}

// 유저 추가
void PartyRoom::PartyRoomAddUser(User* _user)
{
	if (m_partyuser.size() < PARTYMAXUSER)
	{
		m_partyuser.push_back(_user);
		_user->SetPartyNum(partyroom_num);
	}
}

// 유저 삭제
bool PartyRoom::PartyRoomRemoveUser(User* _user)
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (target == _user)
		{
			m_partyuser.remove(target);
			_user->ResetPartyInfo();
			return true;
		}
	}
}

// 유저 삭제(코드)
bool PartyRoom::PartyRoomRemoveUser(char * _code)
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (strcmp(target->GetCurCharacter()->GetCharacter_Code(), _code) == 0)
		{
			target->ResetPartyInfo();
			m_partyuser.remove(target);
			return true;
		}
	}
}

// 파티장 임명
bool PartyRoom::AppointPartyLeader(User* _user)
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (target == _user)
		{
			_user->SetPartyLeader(true);
			return true;
		}
	}
	return false;
}

// 파티장 임명(코드)
bool PartyRoom::AppointPartyLeader(char * _code)
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		if (strcmp(target->GetCurCharacter()->GetCharacter_Code(), _code) == 0)
		{
			target->SetPartyLeader(true);
			return true;
		}
	}
	return false;
}

// 파티장 해임
bool PartyRoom::DismissPartyLeader(User* _user)
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// 그 유저가있고 파티장이면
		if (target == _user && target->isPartyLeader() == true)
		{
			_user->SetPartyLeader(false);
			return true;
		}
	}
	return false;
}

// 파티장 해임(코드)
bool PartyRoom::DismissPartyLeader(char * _code)
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// 그 유저가있고 파티장이면
		if (target->GetCurCharacter()->GetCharacter_Code() == _code && target->isPartyLeader() == true)
		{
			target->SetPartyLeader(false);
			return true;
		}
	}
	return false;
}

// 파티장이 누구인가
User* PartyRoom::WhopartyLeader()
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);
		// 그 유저가있고 파티장이면
		if (target->isPartyLeader() == true)
		{
			return target;
		}
	}
}

// 던전 입장
void PartyRoom::DungeonEnter_PartyRoom()
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);

		target->SetEnterDungeon();
	}
}

// 던전 퇴장
void PartyRoom::DungeonLeave_PartyRoom()
{
	User* target = nullptr;
	for (std::list<User*>::iterator save = m_partyuser.begin(); save != m_partyuser.end(); save++)
	{
		target = (*save);

		target->SetLeaveDungeon();
	}
}

// 몬스터 검색 초기화
void PartyRoom::StartSearchMonsterinfo()
{
	m_monster_control->StartSearchMonsterinfo();
}

// 몬스터 검색
bool PartyRoom::SearchMonsterinfo(MonsterInfo *& _monsterinfo)
{
	return m_monster_control->SearchMonsterinfo(_monsterinfo);
}

// 스테이지 상승
void PartyRoom::RiseStage()
{
	switch (m_dungeon_stage_num)
	{
	case DEFAULT_STAGE:
		m_dungeon_stage_num = DUNGEON_STAGE_NORMAL_1;
		break;
	case DUNGEON_STAGE_NORMAL_1:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_1;
		break;
	case DUNGEON_STAGE_BOSS_1:
		m_dungeon_stage_num = DUNGEON_STAGE_NORMAL_2;
		break;
	case DUNGEON_STAGE_NORMAL_2:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_2;
		break;
	case DUNGEON_STAGE_BOSS_2:
		m_dungeon_stage_num = DUNGEON_STAGE_NORMAL_3;
		break;
	case DUNGEON_STAGE_NORMAL_3:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_3;
		break;
	case DUNGEON_STAGE_BOSS_3:
		m_dungeon_stage_num = DUNGEON_STAGE_BOSS_4;
		break;
	default:
		break;
	}
}

// 스테이지에 맞는 몬스터 정보 셋팅
void PartyRoom::SetDungeonMonsterinfo()
{
	// 스테이지에 맞는 몬스터 정보를 세팅하기전에 데이터가 들어있으면 리셋한다.
	if (m_monster_control->GetMonsterList_Empty() == false)
	{
		m_monster_control->ResetMonsterInfo();
	}

	switch (m_dungeon_stage_num)
	{
	case DUNGEON_STAGE_NORMAL_1:
		m_monster_control->SetFirstStage_NormalMonster();
		break;
	case DUNGEON_STAGE_BOSS_1:
		break;
	case DUNGEON_STAGE_NORMAL_2:
		break;
	case DUNGEON_STAGE_BOSS_2:
		break;
	case DUNGEON_STAGE_NORMAL_3:
		break;
	case DUNGEON_STAGE_BOSS_3:
		break;
	case DUNGEON_STAGE_BOSS_4:
		break;
	default:
		break;
	}
}

// 몬스터 시간 초기화
void PartyRoom::Start_MonsterTimer(int _code, int _num)
{
	m_monster_control->InitMonsterTime(_code, _num);
}

// 몬스터 시간 지났는지 확인
bool PartyRoom::End_MonsterTimer(int _code, int _num)
{
	return m_monster_control->Is_End_MonsterTime(_code, _num);
}

// 몬스터 코드 반환
int PartyRoom::GetMonsterCode(int _count)
{
	return m_monster_control->GetMonsterCode_vector(_count);
}

// 몬스터 정보 요청
bool PartyRoom::GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo *& _monsterinfo)
{
	return m_monster_control->GetMonsterinfo(_monster_code, _monster_num, _monsterinfo);
}

// 몬스터 저장(몬스터코드,몬스터번호) - 새롭게 저장할때
void PartyRoom::SetMonsterinfo(int _monster_code, int _monster_num)
{
	m_monster_control->SetMonsterinfo(_monster_code, _monster_num);
}

// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표,몬스터회전값) - 기본정보에서 수정할때
void PartyRoom::SetMonsterinfo(int _monster_code, int _monster_num, const Vector3 _pos)
{
	m_monster_control->SetMonsterinfo(_monster_code, _monster_num, _pos);
}

// 몬스터 정보 삭제
bool PartyRoom::RemoveMonsterInfo(int _monster_code, int _monster_num)
{
	return m_monster_control->RemoveMonsterInfo(_monster_code,_monster_num);
}

// 몬스터 체력 감소
bool PartyRoom::Monster_HP_Down(int _monster_code, int _monster_num, int _damage)
{
	return m_monster_control->Monster_HP_Down(_monster_code,_monster_num,_damage);
}





// Class PartySystem
PartySystem::PartySystem()
{

}

PartySystem::~PartySystem()
{
	PartyRoom* target = nullptr;

	for (std::list<PartyRoom*>::iterator i = m_partyroom_list.begin(); i != m_partyroom_list.end(); ++i)
	{
		target = (*i);
		delete target;
	}
}

void PartySystem::InitializePartySystem()
{

}

// 리스트 시작부 세팅
void PartySystem::StartSearchPartyRoom()
{
	save = m_partyroom_list.begin();
}

// 유저 추가
bool PartySystem::Partyroom_add_User(User * _user,int _partyroomnum)
{
	PartyRoom* partyroom = nullptr;
	if (_user->isIngame() == true && _user->isParty() == false)
	{
		partyroom = GetPartyRoomSearch(_partyroomnum);

		if (partyroom->GetPartRoomUserNum() >= PARTYMAXUSER && partyroom == nullptr)
		{
			// 파티가 가득찾거나 해당하는 파티방이없으면 실패
			return false;
		}
		// 유저 추가
		partyroom->PartyRoomAddUser(_user);
	}
	return true;
}

// 파티만들기
bool PartySystem::Party_Create(User * _user)
{
	PartyRoom* partyroom = nullptr;

	partyroom = new PartyRoom(UUIDManager::GetInstance()->UUID_PartyRoomNum());

	partyroom->PartyRoomAddUser(_user);
	partyroom->AppointPartyLeader(_user);

	m_partyroom_list.push_back(partyroom);

	return true;
}

// 파티 초대 가능한지
bool PartySystem::Party_Invite(User * _user, User* _targetuser)
{
	PartyRoom* partyroom = nullptr;
	partyroom = GetPartyRoomSearch(_user->GetPartyNum());
	
	if (partyroom == nullptr && _targetuser->isParty() == false)
	{
		// 해당하는 파티가 없으면 실패
		return false;
	}

	return true;
}

// 파티 삭제
bool PartySystem::Party_Remove(User * _user)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;

	if (_user->isPartyLeader() == false)
	{
		// 파티 리더가 아니면 실패
		return false;
	}

	target = GetPartyRoomSearch(_user->GetPartyNum());
	if (target != nullptr)
	{
		target->StartSearchPartyRoom();
		while (target->SearchPartyRoom(user))
		{
			target->PartyRoomRemoveUser(user);
		}

		m_partyroom_list.remove(target);
		delete target;
		target = nullptr;

		return true;
	}

	// 삭제 실패
	return false;
}

// 파티탈퇴
bool PartySystem::Party_User_Leave(User * _user)
{
	PartyRoom* target = nullptr;

	target = GetPartyRoomSearch(_user->GetPartyNum());

	return target->PartyRoomRemoveUser(_user);
}

// 파티 강퇴
bool PartySystem::Party_Kick_User(User * _user, char * _code)
{
	PartyRoom* target = nullptr;

	if (_user->isPartyLeader() == false)
	{
		return false;
	}

	target = GetPartyRoomSearch(_user->GetPartyNum());

	return target->PartyRoomRemoveUser(_code);
}

// 파티장 위임
bool PartySystem::Party_Leader_Delegate(User* _user, char* _code)
{
	PartyRoom* target = nullptr;
	User* user = nullptr;

	if (_user->isPartyLeader() == false)
	{
		return false;
	}
	target = GetPartyRoomSearch(_user->GetPartyNum());

	if (target->DismissPartyLeader(_user) == false)
	{
		return false;
	}

	if (target->AppointPartyLeader(_code) == false)
	{
		target->AppointPartyLeader(_user);
		return false;
	}
	
	return true;
}

// 유저 숫자
int PartySystem::GetPartyRoomUserNum(int _partyroomnum)
{
	PartyRoom* target = nullptr;
	char msg[BUFSIZE];

	target = GetPartyRoomSearch(_partyroomnum);

	// 메세지
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "~GetPartyRoomUserNum :: 유저 숫자[%d] ", target->GetPartRoomUserNum());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	return target->GetPartRoomUserNum();
}

// 파티방 찾기(번호)
PartyRoom * PartySystem::GetPartyRoomSearch(int _partyroomnum)
{
	PartyRoom* target = nullptr;

	for (std::list<PartyRoom*>::iterator save = m_partyroom_list.begin(); save != m_partyroom_list.end(); save++)
	{
		target = (*save);
		if (target->GetPartyRoomNum() == _partyroomnum)
		{
			return target;
		}
	}
}
