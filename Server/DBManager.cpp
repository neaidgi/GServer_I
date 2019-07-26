#include "DBManager.h"
#include "LogManager.h"
#include "ErrorManager.h"
#include "MsgManager.h"

DBManager* DBManager::Instance = nullptr;

DBManager::DBManager()
{
	mysql = nullptr;
}
DBManager::~DBManager()
{
}

void DBManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new DBManager();
	}
}

DBManager * DBManager::GetInstance()
{
	return Instance;
}

void DBManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}
}

bool DBManager::InitializeDB()
{
	mysql = mysql_init(NULL);

	if (mysql_real_connect(mysql, "127.0.0.1", "LoginServer", "1q2w3e4r", "userinfo", 3306, NULL, 0) == NULL)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManager::InitializeDB : ERROR : mysql_real_connect() failed ");
		fprintf(stderr, "Mysql connection error : %s \n", mysql_error(mysql));
		return false;
	}
	else
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManager::InitializeDB : INFO : mysql_real_connect() success ");
		return true;
	}
}

void DBManager::EndManager()
{
	mysql_close(mysql);
}

//DB 선택
bool DBManager::SelectDB(char * _dbname)
{
	if (mysql_select_db(mysql, _dbname)) // 데이터베이스 선택
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManager::SelectDB : ERROR : mysql_select_db() failed ");
		ErrorManager::GetInstance()->err_quit("데이터 베이스 오류");
		return false;
	}
	else
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManager::SelectDB : INFO : mysql_select_db() success ");
		return true;
	}
}

// ID 중복체크
bool DBManager::Login_CheckID(char * _id)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT id FROM User WHERE id =";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/
	sprintf(query, "%s '%s'", base_query, _id);
	/*
	*	끝
	*/

	state = mysql_query(mysql, query);

	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);    //

		/*
		* 결과 값의 다음 로우를 검색한다.
		*/

		bool exist = false;

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row != NULL)     // Result Set 에서 1개씩 배열을 가져옴.
		{
			exist = true;
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return exist;
	}
	else
	{
		fprintf(stderr, "Mysql idcheck error : %s \n", mysql_error(mysql));
		return true;
	}
}

// 회원가입 
bool DBManager::Login_Req_Join(char * _id, char * _pw)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "INSERT INTO User(id,pw) VALUES";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 클라, userinfo 에서 닉네임을 받게 변경 후에 else문 안의 구문만 남기도록

	sprintf(query, "%s ('%s','%s')", base_query, _id, _pw);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql join error : %s \n", mysql_error(mysql));
		return false;
	}
}
// 로그인
bool DBManager::Login_Req_Login(char * _id, char * _pw)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT id, pw FROM User WHERE id";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	//	쿼리문 만들기
	sprintf(query, "%s = '%s' AND pw = '%s'", base_query, _id, _pw);

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		bool result = false;

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row != NULL)     // Result Set 에서 1개씩 배열을 가져옴.
		{
			result = true;
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return result;
	}
	else
	{
		fprintf(stderr, "Mysql join error : %s \n", mysql_error(mysql));
		return false;
	}
}
// 회원탈퇴
bool DBManager::Login_Req_Leave(char * _id)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "DELETE FROM User WHERE id =";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/
	sprintf(query, "%s '%s'", base_query, _id);
	/*
	*	끝
	*/

	state = mysql_query(mysql, query);

	if (state == 0)
	{
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql leave error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 슬롯 개수 요청
bool DBManager::Character_Req_CharacterSlotCount(const char * _id, int& _count)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT COUNT(*) FROM usercharacterinfo WHERE id";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	//	쿼리문 만들기
	sprintf(query, "%s = '%s'", base_query, _id);

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		sql_row = mysql_fetch_row(sql_result);

		// 결과 카운트 저장
		int count = atoi(sql_row[0]);
		_count = count;
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql 슬롯 카운트 에러: %s \n", mysql_error(mysql));
		return false;
	}
}

// 실제 생성한 유저 캐릭터 저장
bool DBManager::Character_CharacterSlotAdd(const char* _id, char* _code, int  _jobcode, const char* _jobname, char* _nick, int _level, int _num)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "INSERT INTO UserCharacterInfo VALUES(";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력
	sprintf(query, "%s'%s', '%s', %d, '%s', '%s', %d, %d);", base_query, _code, _id, _jobcode, _jobname, _nick, _level, _num);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql 캐릭터 추가 에러 : %s \n", mysql_error(mysql));
		return false;
	}
}

// 캐릭터 설계도 요청
// 현재 code받아서 하나씩 외부로 제공하는 함수
// 수정 >> 배열 받아서 모든 캐릭터 정보 output으로 전달 
bool DBManager::Character_Req_CharacterInfo(Character * _character_out[])
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT *";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	{
		sprintf(query, "%s FROM CharacterOrigin", base_query);
	}

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		sql_row = mysql_fetch_row(sql_result);

		Vector3 character_scale;

		// 1번
		character_scale = _character_out[0]->GetScale();
		_character_out[0]->SetCharacter_JobCode(atoi(sql_row[0]));
		_character_out[0]->SetCharacter_JobName(sql_row[1]);
		_character_out[0]->SetCharacter_STR(atoi(sql_row[2]));
		_character_out[0]->SetCharacter_DEX(atoi(sql_row[3]));
		_character_out[0]->SetCharacter_INT(atoi(sql_row[4]));
		_character_out[0]->SetCharacter_HP(atoi(sql_row[5]));
		_character_out[0]->SetCharacter_MP(atoi(sql_row[6]));
		_character_out[0]->SetCharacter_AttackPoint(atoi(sql_row[7]));
		_character_out[0]->SetCharacter_DefensePoint(atoi(sql_row[8]));
		_character_out[0]->SetCharacter_Speed(atoi(sql_row[9]));

		_character_out[0]->SetCharacter_GrowSTR(atoi(sql_row[10]));
		_character_out[0]->SetCharacter_GrowDEX(atoi(sql_row[11]));
		_character_out[0]->SetCharacter_GrowINT(atoi(sql_row[12]));
		_character_out[0]->SetCharacter_GrowHealth(atoi(sql_row[13]));
		_character_out[0]->SetCharacter_GrowMana(atoi(sql_row[14]));
		character_scale.x = atoi(sql_row[15]);
		character_scale.y = atoi(sql_row[16]);
		character_scale.z = atoi(sql_row[17]);
		_character_out[0]->SetScale(character_scale);

		sql_row = mysql_fetch_row(sql_result);

		// 2번
		character_scale = _character_out[1]->GetScale();
		_character_out[1]->SetCharacter_JobCode(atoi(sql_row[0]));
		_character_out[1]->SetCharacter_JobName(sql_row[1]);
		_character_out[1]->SetCharacter_STR(atoi(sql_row[2]));
		_character_out[1]->SetCharacter_DEX(atoi(sql_row[3]));
		_character_out[1]->SetCharacter_INT(atoi(sql_row[4]));
		_character_out[1]->SetCharacter_HP(atoi(sql_row[5]));
		_character_out[1]->SetCharacter_MP(atoi(sql_row[6]));
		_character_out[1]->SetCharacter_AttackPoint(atoi(sql_row[7]));
		_character_out[1]->SetCharacter_DefensePoint(atoi(sql_row[8]));
		_character_out[1]->SetCharacter_Speed(atoi(sql_row[9]));

		_character_out[1]->SetCharacter_GrowSTR(atoi(sql_row[10]));
		_character_out[1]->SetCharacter_GrowDEX(atoi(sql_row[11]));
		_character_out[1]->SetCharacter_GrowINT(atoi(sql_row[12]));
		_character_out[1]->SetCharacter_GrowHealth(atoi(sql_row[13]));
		_character_out[1]->SetCharacter_GrowMana(atoi(sql_row[14]));
		character_scale.x = atoi(sql_row[15]);
		character_scale.y = atoi(sql_row[16]);
		character_scale.z = atoi(sql_row[17]);
		_character_out[1]->SetScale(character_scale);

		sql_row = mysql_fetch_row(sql_result);

		// 3번
		character_scale = _character_out[2]->GetScale();
		_character_out[2]->SetCharacter_JobCode(atoi(sql_row[0]));
		_character_out[2]->SetCharacter_JobName(sql_row[1]);
		_character_out[2]->SetCharacter_STR(atoi(sql_row[2]));
		_character_out[2]->SetCharacter_DEX(atoi(sql_row[3]));
		_character_out[2]->SetCharacter_INT(atoi(sql_row[4]));
		_character_out[2]->SetCharacter_HP(atoi(sql_row[5]));
		_character_out[2]->SetCharacter_MP(atoi(sql_row[6]));
		_character_out[2]->SetCharacter_AttackPoint(atoi(sql_row[7]));
		_character_out[2]->SetCharacter_DefensePoint(atoi(sql_row[8]));
		_character_out[2]->SetCharacter_Speed(atoi(sql_row[9]));

		_character_out[2]->SetCharacter_GrowSTR(atoi(sql_row[10]));
		_character_out[2]->SetCharacter_GrowDEX(atoi(sql_row[11]));
		_character_out[2]->SetCharacter_GrowINT(atoi(sql_row[12]));
		_character_out[2]->SetCharacter_GrowHealth(atoi(sql_row[13]));
		_character_out[2]->SetCharacter_GrowMana(atoi(sql_row[14]));
		character_scale.x = atoi(sql_row[15]);
		character_scale.y = atoi(sql_row[16]);
		character_scale.z = atoi(sql_row[17]);
		_character_out[2]->SetScale(character_scale);

		sql_row = mysql_fetch_row(sql_result);

		// 4번
		character_scale = _character_out[3]->GetScale();
		_character_out[3]->SetCharacter_JobCode(atoi(sql_row[0]));
		_character_out[3]->SetCharacter_JobName(sql_row[1]);
		_character_out[3]->SetCharacter_STR(atoi(sql_row[2]));
		_character_out[3]->SetCharacter_DEX(atoi(sql_row[3]));
		_character_out[3]->SetCharacter_INT(atoi(sql_row[4]));
		_character_out[3]->SetCharacter_HP(atoi(sql_row[5]));
		_character_out[3]->SetCharacter_MP(atoi(sql_row[6]));
		_character_out[3]->SetCharacter_AttackPoint(atoi(sql_row[7]));
		_character_out[3]->SetCharacter_DefensePoint(atoi(sql_row[8]));
		_character_out[3]->SetCharacter_Speed(atoi(sql_row[9]));

		_character_out[3]->SetCharacter_GrowSTR(atoi(sql_row[10]));
		_character_out[3]->SetCharacter_GrowDEX(atoi(sql_row[11]));
		_character_out[3]->SetCharacter_GrowINT(atoi(sql_row[12]));
		_character_out[3]->SetCharacter_GrowHealth(atoi(sql_row[13]));
		_character_out[3]->SetCharacter_GrowMana(atoi(sql_row[14]));
		character_scale.x = atoi(sql_row[15]);
		character_scale.y = atoi(sql_row[16]);
		character_scale.z = atoi(sql_row[17]);
		_character_out[3]->SetScale(character_scale);

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Create error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 캐릭터 슬롯 요청 (번호 받아서 해당 레코드 가져옴)
bool DBManager::Character_Req_CharacterSlot(const char* _id, int _index, int& _jobcode, char * _jobname, char * _nick, int& _level, char * _code)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT *";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	sprintf(query, "%s FROM UserCharacterInfo WHERE id = '%s' AND character_slotnum = %d", base_query, _id, _index);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row == NULL)
		{
			fprintf(stderr, "Mysql 캐릭터 정보 없음 : %s \n", mysql_error(mysql));
			return false;
		}

		// DB 데이터 아웃풋 저장
		// 캐릭터 코드
		memcpy(_code, sql_row[0], strlen(sql_row[0]));

		// 직업코드
		_jobcode = atoi(sql_row[2]);

		memcpy(_jobname, sql_row[3], strlen(sql_row[3]));

		//memcpy(_jobname, sql_row[1], len);

		memcpy(_nick, sql_row[4], strlen(sql_row[4]));

		//memcpy(_nick, sql_row[2], len);

		_level = atoi(sql_row[5]);

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
			mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Create error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 몬스터 설계도 요청
bool DBManager::Character_Req_MonsterInfo(Monster * _monster_out[])
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT *";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	{
		sprintf(query, "%s FROM Monsterinfo", base_query);
	}

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);
		Vector3 monster_scale;

		for (int i = 0; i < MAXMONSTERORIGIN; i++)
		{
			sql_row = mysql_fetch_row(sql_result);
			
			monster_scale = _monster_out[i]->GetScale();

			_monster_out[i]->SetMonster_Code(atoi(sql_row[0]));
			_monster_out[i]->SetMonster_Name(sql_row[1]);
			_monster_out[i]->SetMonster_HP(atoi(sql_row[2]));
			_monster_out[i]->SetMonster_MP(atoi(sql_row[3]));
			_monster_out[i]->SetMonster_AttackPoint(atoi(sql_row[4]));
			_monster_out[i]->SetMonster_DefensePoint(atoi(sql_row[5]));
			monster_scale.x = atoi(sql_row[6]);
			monster_scale.y = atoi(sql_row[7]);
			monster_scale.z = atoi(sql_row[8]);
			_monster_out[i]->SetScale(monster_scale);
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Req_MonsterInfo error : %s \n", mysql_error(mysql));
		return false;
	}

	return true;
}

// 캐릭터 공격(기본공격 및 스킬) 정보 요청
bool DBManager::Character_Req_AttackInfo(Character * _character_out[])
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT *";
	int state = 0;
	int count = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	{
		sprintf(query, "%s FROM characterattackinfo", base_query);
	}

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		while (1)
		{
			// AttackInfo* attackinfo = new AttackInfo();
			AttackInfo attackinfo;
			sql_row = mysql_fetch_row(sql_result);

			if (sql_row == nullptr)
			{
				break;
			}

			count = atoi(sql_row[0]);

			switch (count)
			{
			case 1001:
				attackinfo = _character_out[0]->GetBasicAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atof(sql_row[3]);
				attackinfo.attack_ratio = atof(sql_row[4]);
				_character_out[0]->SetBasicAttack(attackinfo);
				break;
			case 1002:
				attackinfo = _character_out[0]->GetSecondAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atof(sql_row[3]);
				attackinfo.attack_ratio = atof(sql_row[4]);
				_character_out[0]->SetSecondAttack(attackinfo);
				break;
			default:
				break;
			}
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Req_AttackInfo error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 몬스터 공격(기본공격 및 스킬) 정보 요청
bool DBManager::Monster_Req_AttackInfo(Monster * _monster_out[])
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT *";
	int state = 0;
	int count = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	{
		sprintf(query, "%s FROM MonsterAttackInfo", base_query);
	}

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		while (1)
		{
			MonsterAttackInfo attackinfo;
			sql_row = mysql_fetch_row(sql_result);

			if (sql_row == nullptr)
			{
				break;
			}

			count = atoi(sql_row[0]);

			switch (count)
			{
			case SPIDER_FIRST_ATTACK:
				attackinfo = _monster_out[0]->GetFirstAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[0]->SetFirstAttack(attackinfo);
				break;
			case SPIDER_SECOND_ATTACK:
				attackinfo = _monster_out[0]->GetSecondAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[0]->SetSecondAttack(attackinfo);
				break;
			case WORM_FIRST_ATTACK:
				attackinfo = _monster_out[1]->GetFirstAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[1]->SetFirstAttack(attackinfo);
				break;
			case BOSS_SPIDER_FIRST_ATTACK:
				attackinfo = _monster_out[2]->GetFirstAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[2]->SetFirstAttack(attackinfo);
				break;
			case DOG_FIRST_ATTACK:
				attackinfo = _monster_out[3]->GetFirstAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[3]->SetFirstAttack(attackinfo);
				break;
			case ORCCANNONSOLDIER_FIRST_ATTACK:
				attackinfo = _monster_out[4]->GetFirstAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[4]->SetFirstAttack(attackinfo);
				break;
			case BEAR_FIRST_ATTACK:
				attackinfo = _monster_out[6]->GetFirstAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[6]->SetFirstAttack(attackinfo);
				break;
			case DINOSAUR_FIRST_ATTACK:
				attackinfo = _monster_out[7]->GetFirstAttack();
				attackinfo.attack_code = atoi(sql_row[0]);
				attackinfo.attack_angle = atoi(sql_row[2]);
				attackinfo.attack_range = atoi(sql_row[3]);
				attackinfo.attack_ratio = atoi(sql_row[4]);
				attackinfo.attack_type = atoi(sql_row[5]);
				_monster_out[7]->SetFirstAttack(attackinfo);
				break;
			default:
				break;
			}
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Monster_Req_AttackInfo error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 스테이지 정보 요청
bool DBManager::Stage_Req_StageInfo(StageInfo * _stage_out[])
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT *";
	int state = 0;
	int count = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	{
		sprintf(query, "%s FROM stageinfo", base_query);
	}

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		while (1)
		{
			sql_row = mysql_fetch_row(sql_result);

			if (sql_row == nullptr)
			{
				break;
			}

			count = atoi(sql_row[1]);

			switch (count)
			{
			case 1:
				_stage_out[0]->SetStage_Num(atoi(sql_row[1]));
				break;
			case 2:
				_stage_out[1]->SetStage_Num(atoi(sql_row[1]));
				break;
			case 3:
				_stage_out[2]->SetStage_Num(atoi(sql_row[1]));
				break;
			default:
				break;
			}
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Monster_Req_AttackInfo error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 스테이지 몬스터 정보 요청
bool DBManager::Stage_Req_StageMonsterInfo(StageInfo * _stage_out[])
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT *";
	int state = 0;
	int id = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	{
		sprintf(query, "%s FROM stagemonsterinfo", base_query);
	}

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		while (1)
		{
			sql_row = mysql_fetch_row(sql_result);

			if (sql_row == nullptr)
			{
				break;
			}

			id = atoi(sql_row[0]);

			switch (id)
			{
			case 10:
				_stage_out[0]->SetStage_NormalFisrtMonster_Code(atoi(sql_row[1]));
				_stage_out[0]->SetStage_NormalFisrtMonster_Num(atoi(sql_row[2]));
				break;
			case 11:
				_stage_out[0]->SetStage_NormalSecondMonster_Code(atoi(sql_row[1]));
				_stage_out[0]->SetStage_NormalSecondMonster_Num(atoi(sql_row[2]));
				break;
			case 12:
				_stage_out[0]->SetStage_BossMonster_Code(atoi(sql_row[1]));
				break;
			case 20:
				_stage_out[1]->SetStage_NormalFisrtMonster_Code(atoi(sql_row[1]));
				_stage_out[1]->SetStage_NormalFisrtMonster_Num(atoi(sql_row[2]));
				break;
			case 21:
				_stage_out[1]->SetStage_NormalSecondMonster_Code(atoi(sql_row[1]));
				_stage_out[1]->SetStage_NormalSecondMonster_Num(atoi(sql_row[2]));
				break;
			case 22:
				_stage_out[1]->SetStage_BossMonster_Code(atoi(sql_row[1]));
				break;
			case 30:
				_stage_out[2]->SetStage_NormalFisrtMonster_Code(atoi(sql_row[1]));
				_stage_out[2]->SetStage_NormalFisrtMonster_Num(atoi(sql_row[2]));
				break;
			case 31:
				_stage_out[2]->SetStage_NormalSecondMonster_Code(atoi(sql_row[1]));
				_stage_out[2]->SetStage_NormalSecondMonster_Num(atoi(sql_row[2]));
				break;
			case 32:
				_stage_out[2]->SetStage_BossMonster_Code(atoi(sql_row[1]));
				break;
			default:
				break;
			}
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Monster_Req_AttackInfo error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 캐릭터 삭제
bool DBManager::Character_Req_CharacterDelete(const char * _id, int _index)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "DELETE FROM UserCharacterInfo WHERE id = ";

	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 //
	sprintf(query, "%s '%s' AND character_slotnum = %d", base_query, _id, _index);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Delete error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 캐릭터 닉네임 중복확인
bool DBManager::Character_Req_CharacterCheckName(const char * _nick)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT character_nickname FROM UserCharacterInfo WHERE character_nickname = ";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/
	sprintf(query, "%s '%s'", base_query, _nick);
	/*
	*	끝
	*/

	state = mysql_query(mysql, query);

	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);    //

		/*
		* 결과 값의 다음 로우를 검색한다.
		*/

		bool exist = false;

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row != NULL)     // Result Set 에서 1개씩 배열을 가져옴.
		{
			exist = true;
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return exist;
	}
	else
	{
		fprintf(stderr, "Mysql ChracterNickCheck error : %s \n", mysql_error(mysql));
		return true;
	}
}

// 
bool DBManager::Character_Req_CharacterName(const char * _id, int _index, char* _nick)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT ";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	sprintf("%s character_nickname FROM usercharacterinfo WHERE user_id = '%s' AND character_slotnum = '%d'", base_query, _id, _index);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row == NULL)
		{
			return false;
		}

		// DB 데이터 아웃풋 저장
		memcpy(_nick, sql_row[0], NICKNAMESIZE);

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Req_CharacterName error : %s \n", mysql_error(mysql));
		return false;
	}
}
// 캐릭터 위치 요청
bool DBManager::Character_Req_CharacterPos(char* _code, Vector3& _pos)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT character_pos_x FROM characterpos  WHERE character_code = ";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 
	sprintf(query, "%s %d", base_query, _code);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row == nullptr)
		{
			fprintf(stderr, "Mysql 해당캐릭터 위치 없음 : %s \n", mysql_error(mysql));
			return false;
		}

		if (sql_row[0] == NULL || !sql_row[1])
		{
			return false;
		}

		// DB 데이터 아웃풋 저장
		_pos.x = atof(sql_row[1]);
		_pos.y = atof(sql_row[2]);
		_pos.z = atof(sql_row[3]);

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Pos error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 캐릭터 위치 추가
bool DBManager::Charactor_CharacterPosAdd(int _code)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "INSERT INTO CharacterPos VALUES";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 
	sprintf(query, "%s (%d, null, null, null)", base_query, _code);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Pos error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 삭제한 유저캐릭터 슬롯 뒤에 슬롯이 몇개있는지
bool DBManager::Character_Req_SlotCount(const char * _id, int _slotnum, int& _index)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT COUNT(*) FROM  usercharacterinfo WHERE id = ";

	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 //
	sprintf(query, "%s '%s' AND character_slotnum > %d ", base_query, _id, _slotnum);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		sql_result = mysql_store_result(mysql);

		sql_row = mysql_fetch_row(sql_result);

		if (sql_row == NULL)
		{
			return false;
		}

		// DB 데이터 아웃풋 저장
		_index = atoi(sql_row[0]);

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Req_SlotCount error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 슬롯 당기기
bool DBManager::Character_Slot_Pull(const char * _id, int _afterslot, int _beforeslot)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "UPDATE usercharacterinfo SET character_slotnum = ";

	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 //
	sprintf(query, "%s %d WHERE id= '%s' AND character_slotnum = %d ", base_query, _beforeslot, _id, _afterslot);

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, query);

	// 성공
	if (state == 0)
	{
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Slot_Pull error : %s \n", mysql_error(mysql));
		return false;
	}
}

bool DBManager::Character_Save(const char* _id, Character* _character)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "  UPDATE usercharacterinfo  SET character_level =  ";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/
	sprintf(query, "%s  %d WHERE id= '%s' AND character_code = '%s' ", base_query, _character->GetCharacter_Level(), _id, _character->GetCharacter_Code());
	/*
	*	끝
	*/

	state = mysql_query(mysql, query);

	if (state == 0)
	{
		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Save error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 캐릭터 스폰위치 요청
bool DBManager::Charactor_Req_CharacterSpawnPos(Vector3 * _pos, int& _count)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT * FROM SpawnPos";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, base_query);
	MsgManager::GetInstance()->DisplayMsg("DB", "플레이어 스폰 위치 요청 중");
	// 성공
	if (state == 0)
	{
		MsgManager::GetInstance()->DisplayMsg("DB", "플레이어 스폰 위치 요청 성공");
		sql_result = mysql_store_result(mysql);

		int i = 0;

		while (1)
		{
			sql_row = mysql_fetch_row(sql_result);
			if (sql_row == NULL)
			{
				break;
			}

			_pos[i].x = atof(sql_row[1]);
			_pos[i].y = atof(sql_row[2]);
			_pos[i].z = atof(sql_row[3]);

			i++;
		}

		_count = mysql_num_rows(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Charactor_Req_CharacterSpawnPos error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 던전 캐릭터 스폰위치 요청
bool DBManager::Charactor_Req_DungeonSpawnPos(Vector3 * _pos, int & _count)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT * FROM dungeonspawnpos";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, base_query);
	MsgManager::GetInstance()->DisplayMsg("DB", "던전 스폰 위치 요청 중");
	// 성공
	if (state == 0)
	{
		MsgManager::GetInstance()->DisplayMsg("DB", "던전 스폰 위치 요청 성공");
		sql_result = mysql_store_result(mysql);

		int i = 0;

		while (1)
		{
			sql_row = mysql_fetch_row(sql_result);
			if (sql_row == NULL)
			{
				break;
			}

			_pos[i].x = atof(sql_row[1]);
			_pos[i].y = atof(sql_row[2]);
			_pos[i].z = atof(sql_row[3]);

			i++;
		}

		_count = mysql_num_rows(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Charactor_Req_DungeonSpawnPos error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 던전 스테이지 스폰위치 요청
bool DBManager::Charactor_Req_DungeonStageSpawnPos(Vector3 * _pos, int & _count)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT * FROM DungeonStageSpawnpos";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, base_query);
	MsgManager::GetInstance()->DisplayMsg("DB", "던전 스테이지 스폰 위치 요청 중");
	// 성공
	if (state == 0)
	{
		MsgManager::GetInstance()->DisplayMsg("DB", "던전 스테이지 스폰 위치 요청 성공");
		sql_result = mysql_store_result(mysql);

		int i = 0;

		while (1)
		{
			sql_row = mysql_fetch_row(sql_result);
			if (sql_row == NULL)
			{
				break;
			}

			_pos[i].x = atof(sql_row[1]);
			_pos[i].y = atof(sql_row[2]);
			_pos[i].z = atof(sql_row[3]);

			i++;
		}

		_count = mysql_num_rows(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Charactor_Req_DungeonStageSpawnPos error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 몬스터 스폰위치 요청
bool DBManager::Charactor_Req_MonsterSpawnPos(Vector3 * _pos, int & _count)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT * FROM MonsterSpawnPos";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	끝
	*/

	// 쿼리 날리기
	state = mysql_query(mysql, base_query);
	MsgManager::GetInstance()->DisplayMsg("DB", "몬스터 스폰 위치 요청 중");
	// 성공
	if (state == 0)
	{
		MsgManager::GetInstance()->DisplayMsg("DB", "몬스터 스폰 위치 요청 성공");
		sql_result = mysql_store_result(mysql);

		int i = 0;

		while (1)
		{
			sql_row = mysql_fetch_row(sql_result);
			if (sql_row == NULL)
			{
				break;
			}

			_pos[i].x = atof(sql_row[1]);
			_pos[i].y = atof(sql_row[2]);
			_pos[i].z = atof(sql_row[3]);

			i++;
		}

		_count = mysql_num_rows(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Charactor_Req_MonsterSpawnPos error : %s \n", mysql_error(mysql));
		return false;
	}
}

