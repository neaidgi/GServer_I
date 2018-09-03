#include "DBManager.h"
#include "LogManager.h"
#include "ErrorManager.h"

DBManger* DBManger::Instance = nullptr;

DBManger::DBManger()
{
	mysql = nullptr;
}
DBManger::~DBManger()
{
}

void DBManger::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new DBManger();
	}
}

DBManger * DBManger::GetInstance()
{
	return Instance;
}

void DBManger::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
	}
}

bool DBManger::InitializeDB()
{
	mysql = mysql_init(NULL);

	if (mysql_real_connect(mysql, "127.0.0.1", "LoginServer", "1q2w3e4r", "userinfo", 3306, NULL, 0) == NULL)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManger::InitializeDB : ERROR : mysql_real_connect() failed ");
		fprintf(stderr, "Mysql connection error : %s \n", mysql_error(mysql));
		return false;
	}
	else
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManger::InitializeDB : INFO : mysql_real_connect() success ");
		return true;
	}
}

void DBManger::EndManager()
{
	mysql_close(mysql);
}

bool DBManger::SelectDB(char * _dbname)
{
	if (mysql_select_db(mysql, _dbname)) // 데이터베이스 선택
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManger::SelectDB : ERROR : mysql_select_db() failed ");
		ErrorManager::GetInstance()->err_quit("데이터 베이스 오류");
		return false;
	}
	else
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("DBManger::SelectDB : INFO : mysql_select_db() success ");
		return true;
	}
}

bool DBManger::Login_CheckID(char * _id)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT id FROM info WHERE id =";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/
	sprintf(query,"%s '%s'", base_query, _id);
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

		if(sql_row != NULL)     // Result Set 에서 1개씩 배열을 가져옴.
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

bool DBManger::Login_reqJoin(char * _id, char * _pw, char * _nick)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "INSERT INTO info values";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 클라, userinfo 에서 닉네임을 받게 변경 후에 else문 안의 구문만 남기도록
	if (_nick == NULL)
	{
		sprintf(query, "%s ('%s','%s','%s')", base_query, _id, _pw, "0");
	}
	else
	{
		sprintf(query, "%s ('%s','%s','%s')", base_query, _id, _pw, _nick);
	}
	
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
		fprintf(stderr, "Mysql join error : %s \n" , mysql_error(mysql));
		return false;
	}
}

bool DBManger::Login_reqLogin(char * _id, char * _pw)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT id, pw FROM info WHERE id";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	//	쿼리문 만들기
	sprintf(query, "%s = '%s' AND pw = '%s'", base_query, _id , _pw);

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

bool DBManger::Login_reqLeave(char * _id)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "DELETE FROM info WHERE id =";
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

// 실제 생성한 유저 캐릭터 저장
bool DBManger::Character_CharacterSlotAdd(const char* _id, int _index, int _code, char * _jobname, char * _nick, int _level)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "INSERT INTO UserCharacterInfo";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	switch(_index)
	{
	case 1:
		sprintf(query, "%s (%d,'%s','%s',%d) SELECT character_code_first, character_jobname_first, character_nickname_first, character_level_first WHERE id = %s", base_query, _code, _jobname, _nick, _level, _id);
		break;
	case 2:
		sprintf(query, "%s (%d,'%s','%s',%d) SELECT character_code_second, character_jobname_second, character_nickname_second, character_level_second WHERE id = %s", base_query, _code, _jobname, _nick, _level, _id);
		break;
	case 3:
		sprintf(query, "%s (%d,'%s','%s',%d) SELECT character_code_third, character_jobname_third, character_nickname_third, character_level_third WHERE id = %s", base_query, _code, _jobname, _nick, _level, _id);
		break;
	}

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
		fprintf(stderr, "Mysql Character_Create error : %s \n", mysql_error(mysql));
		return false;
	}
}

// 캐릭터 설계도 요청
// 현재 code받아서 하나씩 외부로 제공하는 함수
// 수정 >> 배열 받아서 모든 캐릭터 정보 output으로 전달 
bool DBManger::Character_reqCharacterInfo(Character * _character_out[])
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
		sprintf(query, "%s FROM CharacterInfo", base_query);
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

		// 1번
		_character_out[0]->SetCharacter_Code(*(int*)sql_row[0]);
		_character_out[0]->SetCharacter_Name(sql_row[1]);
		_character_out[0]->SetCharacter_STR(*(int*)sql_row[2]);
		_character_out[0]->SetCharacter_DEX(*(int*)sql_row[3]);
		_character_out[0]->SetCharacter_INT(*(int*)sql_row[4]);
		_character_out[0]->SetCharacter_Health(*(int*)sql_row[5]);
		_character_out[0]->SetCharacter_Mana(*(int*)sql_row[6]);
		_character_out[0]->SetCharacter_AttackPoint(*(int*)sql_row[7]);
		_character_out[0]->SetCharacter_DefensePoint(*(int*)sql_row[8]);
		_character_out[0]->SetCharacter_Speed(*(int*)sql_row[9]);

		_character_out[0]->SetCharacter_GrowHealth(*(int*)sql_row[10]);
		_character_out[0]->SetCharacter_GrowMana(*(int*)sql_row[11]);
		_character_out[0]->SetCharacter_GrowSTR(*(int*)sql_row[12]);
		_character_out[0]->SetCharacter_GrowDEX(*(int*)sql_row[13]);
		_character_out[0]->SetCharacter_GrowINT(*(int*)sql_row[14]);

		sql_row = mysql_fetch_row(sql_result);

		// 2번
		_character_out[1]->SetCharacter_Code(*(int*)sql_row[0]);
		_character_out[1]->SetCharacter_Name(sql_row[1]);
		_character_out[1]->SetCharacter_STR(*(int*)sql_row[2]);
		_character_out[1]->SetCharacter_DEX(*(int*)sql_row[3]);
		_character_out[1]->SetCharacter_INT(*(int*)sql_row[4]);
		_character_out[1]->SetCharacter_Health(*(int*)sql_row[5]);
		_character_out[1]->SetCharacter_Mana(*(int*)sql_row[6]);
		_character_out[1]->SetCharacter_AttackPoint(*(int*)sql_row[7]);
		_character_out[1]->SetCharacter_DefensePoint(*(int*)sql_row[8]);
		_character_out[1]->SetCharacter_Speed(*(int*)sql_row[9]);

		_character_out[1]->SetCharacter_GrowHealth(*(int*)sql_row[10]);
		_character_out[1]->SetCharacter_GrowMana(*(int*)sql_row[11]);
		_character_out[1]->SetCharacter_GrowSTR(*(int*)sql_row[12]);
		_character_out[1]->SetCharacter_GrowDEX(*(int*)sql_row[13]);
		_character_out[1]->SetCharacter_GrowINT(*(int*)sql_row[14]);

		sql_row = mysql_fetch_row(sql_result);

		// 3번
		_character_out[2]->SetCharacter_Code(*(int*)sql_row[0]);
		_character_out[2]->SetCharacter_Name(sql_row[1]);
		_character_out[2]->SetCharacter_STR(*(int*)sql_row[2]);
		_character_out[2]->SetCharacter_DEX(*(int*)sql_row[3]);
		_character_out[2]->SetCharacter_INT(*(int*)sql_row[4]);
		_character_out[2]->SetCharacter_Health(*(int*)sql_row[5]);
		_character_out[2]->SetCharacter_Mana(*(int*)sql_row[6]);
		_character_out[2]->SetCharacter_AttackPoint(*(int*)sql_row[7]);
		_character_out[2]->SetCharacter_DefensePoint(*(int*)sql_row[8]);
		_character_out[2]->SetCharacter_Speed(*(int*)sql_row[9]);

		_character_out[2]->SetCharacter_GrowHealth(*(int*)sql_row[10]);
		_character_out[2]->SetCharacter_GrowMana(*(int*)sql_row[11]);
		_character_out[2]->SetCharacter_GrowSTR(*(int*)sql_row[12]);
		_character_out[2]->SetCharacter_GrowDEX(*(int*)sql_row[13]);
		_character_out[2]->SetCharacter_GrowINT(*(int*)sql_row[14]);

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

bool DBManger::Character_reqCharacterSlot(const char* _id, int _index, int* _code, const char* _jobname, char * _nick, int* _level)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	switch (_index)
	{
	case 1: 
		sprintf(query, 
			"%s character_code_first, character_jobname_first, character_nickname_first, character_level_first FROM UserCharacterInfo WHERE user_id = %s", base_query, _id);
		break;
	case 2:
		sprintf(query, 
			"%s character_code_second, character_jobname_second, character_nickname_second, character_level_second FROM UserCharacterInfo WHERE user_id = %s", base_query, _id);
		break;
	case 3:
		sprintf(query, 
			"%s character_code_third, character_jobname_third, character_nickname_third, character_level_third FROM UserCharacterInfo WHERE user_id = %s", base_query, _id);
		break;
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

		if (sql_row == NULL)
		{
			return false;
		}

		// DB 데이터 아웃풋 저장
		*_code = *(int*)sql_row[0];
		memcpy((char*)_jobname, sql_row[1], strlen(sql_row[1]));
		memcpy(_nick, sql_row[2], strlen(sql_row[2]));
		*_level = *(int*)sql_row[3];

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

bool DBManger::Character_reqCharacterDelete(const char * _id, int _index)
{
		MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "UPDATE";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/

	// 쿼리 입력 // code, jobname, nick, level
	switch (_index)
	{
	case 1: 
		sprintf(query, 
			"%s UserCharacterInfo SET character_code_first = NULL,character_jobname_first = NULL,character_nickname_first = NULL, character_level_first = NULL WHERE user_id = '%s'", base_query, _id);
		break;
	case 2:
		sprintf(query, 
			"%s UserCharacterInfo SET character_code_second = NULL,character_jobname_second = NULL,character_nickname_second = NULL, character_level_second = NULL WHERE user_id = %s", base_query, _id);
		break;
	case 3:
		sprintf(query, 
			"%s UserCharacterInfo SET character_code_third = NULL,character_jobname_third = NULL,character_nickname_third = NULL, character_level_third = NULL WHERE user_id = %s", base_query, _id);
		break;
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

		if (sql_row == NULL)
		{
			return false;
		}

		/*
		* result 지시자와 관련된 점유 메모리를 해제한다.
		*/
		mysql_free_result(sql_result);

		return true;
	}
	else
	{
		fprintf(stderr, "Mysql Character_Delete error : %s \n", mysql_error(mysql));
		return false;
	}
}

bool DBManger::Character_reqCharacterCheckName(const char * _nick)
{
	MYSQL_RES *sql_result;  // the results
	MYSQL_ROW sql_row;      // the results row (line by line)

	char* base_query = "SELECT character_nickname_first,character_nickname_second,character_nickname_third FROM usercharacterinfo WHERE character_nickname_first =";
	int state = 0;

	char query[255];
	memset(query, 0, sizeof(query));

	/*
	*	쿼리문 만들기
	*/
	sprintf(query, "%s '%s' OR character_nickname_second = '%s' OR character_nickname_third = '%s'", base_query, _nick, _nick, _nick);
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

