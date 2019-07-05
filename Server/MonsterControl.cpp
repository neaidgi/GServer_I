#include "MonsterControl.h"

MonsterInfo::MonsterInfo()
{
	m_monster_time = nullptr;
	m_monster = nullptr;
	m_monster_info_num = -1;
	m_monster_activate = true;
}

MonsterInfo::~MonsterInfo()
{
	if (m_monster_time != nullptr)
	{
		delete m_monster_time;
		m_monster_time = nullptr;
	}

	if (m_monster != nullptr)
	{
		delete m_monster;
		m_monster = nullptr;
	}
}

// 몬스터 타이머 시간초기화
void MonsterInfo::InitMonsterTime()
{
	m_monster_time->Start_Time();
}

// 몬스터 시간이 지났는지
bool MonsterInfo::Is_End_MonsterTime()
{
	double time = m_monster_time->End_Time();
	
	// 메세지
	//char msg[BUFSIZE];
	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, " 몬스터 타임 [%lf]", time );
	//MsgManager::GetInstance()->DisplayMsg("TIME", msg);

	if (time >= MONSTERTIME)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// MonsterControl
MonsterControl::MonsterControl()
{
	number_monster_types = 0;
}

MonsterControl::~MonsterControl()
{
	MonsterInfo* target = nullptr;

	for (m_monsterinfo_save = m_monsterinfo_list.begin(); m_monsterinfo_save != m_monsterinfo_list.end(); ++m_monsterinfo_save)
	{
		target = (*m_monsterinfo_save);
		delete target;
	}
}

// 검색 초기화
void MonsterControl::StartSearchMonsterinfo()
{
	m_monsterinfo_save = m_monsterinfo_list.begin();
}

// 검색
bool MonsterControl::SearchMonsterinfo(MonsterInfo *& _monsterinfo)
{
	if (m_monsterinfo_save != m_monsterinfo_list.end())
	{
		_monsterinfo = *m_monsterinfo_save;
		++m_monsterinfo_save;
		return true;
	}
	else
	{
		return false;
	}
}

// 몬스터 코드 추가
void MonsterControl::AddMonsterCode_vector(MONSTER_ORIGINCODE _code)
{
	m_monstercode_vector.push_back(_code);
}

// 몬스터 코드 반환
MONSTER_ORIGINCODE MonsterControl::GetMonsterCode_vector(int _count)
{
	return m_monstercode_vector.at(_count);
}

// 몬스터 실제 조립
Monster * MonsterControl::MonsterSelect(int _monster_code)
{
	Monster* monster = new Monster();
	const Monster* origin_monster = nullptr;
	GameDataManager::GetInstance()->Monster_Origin_Data(_monster_code, origin_monster);

	monster->SetMonster_AttackPoint(origin_monster->GetMonster_AttackPoint());
	monster->SetMonster_Code(origin_monster->GetMonster_Code());
	monster->SetMonster_DefensePoint(origin_monster->GetMonster_DefensePoint());
	monster->SetMonster_HP(origin_monster->GetMonster_HP());
	monster->SetMonster_MP(origin_monster->GetMonster_MP());
	monster->SetMonster_Current_HP(origin_monster->GetMonster_HP());
	monster->SetMonster_Current_MP(origin_monster->GetMonster_MP());
	monster->SetMonster_Name(origin_monster->GetMonster_Name());
	monster->SetMonster_Num(origin_monster->GetMonster_Num());
	monster->SetMonster_Speed(origin_monster->GetMonster_Speed());
	monster->SetPosition(origin_monster->GetPosition());
	monster->SetRotation(origin_monster->GetRotation());
	monster->SetScale(origin_monster->GetScale());
	monster->SetFirstAttack(origin_monster->GetFirstAttack());
	monster->SetSecondAttack(origin_monster->GetSecondAttack());
	return monster;
}

// 몬스터 타이머 추가
MonsterTime* MonsterControl::AddMonsterTimer()
{
	MonsterTime* monstertime = new MonsterTime();
	return monstertime;
}

// 몬스터 타이머 시간초기화
void MonsterControl::InitMonsterTime(int _code, int _num)
{
	MonsterInfo* monsterinfo = nullptr;
	GetMonsterinfo(_code, _num, monsterinfo);

	monsterinfo->InitMonsterTime();
}

// 몬스터 시간이 지났는지
bool MonsterControl::Is_End_MonsterTime(int _code, int _num)
{
	MonsterInfo* monsterinfo = nullptr;
	GetMonsterinfo(_code, _num, monsterinfo);

	return 	monsterinfo->Is_End_MonsterTime();
}

// 몬스터 정보 검색
bool MonsterControl::GetMonsterinfo(int _monster_code, int _monster_num, MonsterInfo *& _monsterinfo)
{
	MonsterInfo* monsterinfo = nullptr;

	StartSearchMonsterinfo();
	while (SearchMonsterinfo(monsterinfo))
	{
		if (monsterinfo->GetMonster()->GetMonster_Code() == _monster_code && monsterinfo->GetMonsterNum() == _monster_num)
		{
			_monsterinfo = monsterinfo;
			return true;
		}
	}

	return false;
}

// 몬스터 저장(몬스터코드,몬스터번호) - 새롭게 저장할때
void MonsterControl::SetMonsterinfo(int _monster_code, int _monster_num)
{
	MonsterInfo* monsterinfo = new MonsterInfo();

	monsterinfo->SetMonster(MonsterSelect(_monster_code));
	monsterinfo->SetMonsterNum(_monster_num);
	monsterinfo->SetMonsterTime(AddMonsterTimer());

	monsterinfo->InitMonsterTime();

	m_monsterinfo_list.push_back(monsterinfo);
}

// 몬스터 정보 저장(몬스터코드,몬스터번호,몬스터좌표,몬스터회전값) - 기본정보에서 수정할때
void MonsterControl::SetMonsterinfo(int _monster_code, int _monster_num,const Vector3 _pos)
{
	MonsterInfo* monsterinfo = nullptr;
	GetMonsterinfo(_monster_code, _monster_num, monsterinfo);
	monsterinfo->GetMonster()->SetPosition(_pos);
	//monsterinfo->GetMonster()->SetRotation(_rot);
}

// 몬스터 정보 삭제
bool MonsterControl::RemoveMonsterInfo(int _monster_code, int _monster_num)
{
	MonsterInfo* monsterinfo = nullptr;

	StartSearchMonsterinfo();
	while (SearchMonsterinfo(monsterinfo))
	{
		if (monsterinfo->GetMonster()->GetMonster_Code() == _monster_code && monsterinfo->GetMonsterNum() == _monster_num)
		{
			m_monsterinfo_list.remove(monsterinfo);
			delete monsterinfo;
			monsterinfo = nullptr;
			return true;
		}
	}

	return false;
}

// 몬스터 리스트 초기화(스테이지 클리어 or 새로운 상태일때 리스트를 초기화하고 새로운 몹을 넣기전 작업)
void MonsterControl::ResetMonsterInfo()
{
	MonsterInfo* target = nullptr;
	MONSTER_ORIGINCODE code;

	for (m_monsterinfo_save = m_monsterinfo_list.begin(); m_monsterinfo_save != m_monsterinfo_list.end(); ++m_monsterinfo_save)
	{
		target = (*m_monsterinfo_save);
		m_monsterinfo_list.remove(target);
		delete target;
	}

	// 메모리에 문제있을듯 수정해야함
	m_monstercode_vector.clear();
}

// 리스트 비어있는지(비어있으면 true 아니면 false)
bool MonsterControl::GetMonsterList_Empty()
{
	return m_monsterinfo_list.empty();
}

// 첫번째 스테이지 일반몹 저장
void MonsterControl::SetFirstStage_NormalMonster()
{
	// 거미 저장
	for (int i = 0; i < FIRSTSTAGE_NORMALMONSTER_1; i++)
	{
		SetMonsterinfo(SPIDER, i);
	}
	// 애벌래 저장
	//for (int i = 0; i < FIRSTSTAGE_NORMALMONSTER_1; i++)
	//{
	//	SetMonsterinfo(WORM, i);
	//}

	AddMonsterCode_vector(SPIDER);
	//AddMonsterCode_vector(WORM);

	number_monster_types = m_monstercode_vector.size();
}

// 몬스터 체력 감소
bool MonsterControl::Monster_HP_Down(int _monster_code, int _monster_num, int _damage)
{
	MonsterInfo* monsterinfo = nullptr;

	if (GetMonsterinfo(_monster_code, _monster_num, monsterinfo) == false)
	{
		return false;
	}

	CriticalSectionManager::GetInstance()->Enter();
	float hp = monsterinfo->GetMonster()->GetMonster_Current_HP() - _damage;

	// 죽으면 false
	if (hp <= 0)
	{
		monsterinfo->GetMonster()->SetMonster_Current_HP(0);
		monsterinfo->SetMonsterDie();
		CriticalSectionManager::GetInstance()->Leave();
		return false;
	}
	else // 살아있으면 true
	{
		monsterinfo->GetMonster()->SetMonster_Current_HP(hp);
		CriticalSectionManager::GetInstance()->Leave();
		return true;
	}
}

