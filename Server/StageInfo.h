#ifndef _STAGE_INFO_H
#define _STAGE_INFO_H

#include "Global.h"

// DB에서 가져온 정보 저장할 스테이지 정보 객체
class StageInfo
{
private:
	// 스테이지 번호
	int m_stage_num;
	// 일반몹 몇마리인지
	int m_normal_monster_num[STAGE_MONSTER_NUM];
	// 일반몹 코드
	int m_normal_monster_code[STAGE_MONSTER_NUM];
	// 보스몹 코드
	int m_boss_monster_code;

public:

	int GetStage_Num() const { return m_stage_num; }
	void GetStage_NormalMonster_Num(int _normalmonster[]) const
	{
		_normalmonster[0] = m_normal_monster_num[0];
		_normalmonster[1] = m_normal_monster_num[1];
	}

	void GetStage_NormalMonster(int _normalmonster[]) const 
	{
		_normalmonster[0] = m_normal_monster_code[0]; 
		_normalmonster[1] = m_normal_monster_code[1];
	}
	int GetStage_BossMonster() const { return m_boss_monster_code; }


	void SetStage_Num(int _stage_num) { m_stage_num = _stage_num; }
	void SetStage_NormalMonster_Num(int _normalmonster_num[])
	{ 
		m_normal_monster_num[0] = _normalmonster_num[0]; 
		m_normal_monster_num[1] = _normalmonster_num[1];
	}
	// set 첫번째 일반몹 숫자
	void SetStage_NormalFisrtMonster_Num(int _firstmonster_num)
	{
		m_normal_monster_num[0] = _firstmonster_num;
	}
	// set 두번째 일반몹 숫자
	void SetStage_NormalSecondMonster_Num(int _secondmonster_num)
	{
		m_normal_monster_num[1] = _secondmonster_num;
	}

	void SetStage_NormalMonster_Code(int _normalmonster[])
	{
		m_normal_monster_code[0] = _normalmonster[0]; 
		m_normal_monster_code[1] = _normalmonster[1];
	}
	// set 첫번째 일반몹 코드
	void SetStage_NormalFisrtMonster_Code(int _firstmonster_code)
	{
		m_normal_monster_code[0] = _firstmonster_code;
	}
	// set 두번째 일반몹 코드
	void SetStage_NormalSecondMonster_Code(int _secondmonster_code)
	{
		m_normal_monster_code[1] = _secondmonster_code;
	}

	void SetStage_BossMonster_Code(const int& _bossmonster_code) { m_boss_monster_code = _bossmonster_code; }


	StageInfo()
	{
		m_stage_num = 0;
		m_normal_monster_num[0] = 0;
		m_normal_monster_num[1] = 0;
		m_normal_monster_code[0] = 0;
		m_normal_monster_code[1] = 0;
	}
	~StageInfo()
	{

	}

	StageInfo & StageInfo::operator=(const StageInfo & rhs)
	{
		m_stage_num = rhs.m_stage_num;
		m_normal_monster_num[0] = rhs.m_normal_monster_num[0];
		m_normal_monster_num[1] = rhs.m_normal_monster_num[1];
		m_normal_monster_code[0] = rhs.m_normal_monster_code[0];
		m_normal_monster_code[1] = rhs.m_normal_monster_code[1];
		m_boss_monster_code = rhs.m_boss_monster_code;
		return *this;
	}

};

#endif