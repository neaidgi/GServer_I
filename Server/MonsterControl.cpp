#include "MonsterControl.h"

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
	monster->SetMonster_Health(origin_monster->GetMonster_Health());
	monster->SetMonster_Mana(origin_monster->GetMonster_Mana());
	monster->SetMonster_Name(origin_monster->GetMonster_Name());
	monster->SetMonster_Num(origin_monster->GetMonster_Num());
	monster->SetMonster_Speed(origin_monster->GetMonster_Speed());
	monster->SetPosition(origin_monster->GetPosition());
	monster->SetRotation(origin_monster->GetRotation());
	monster->SetScale(origin_monster->GetScale());

	return monster;
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

	/*number_monster_types = m_monstercode_vector.size();*/
}
