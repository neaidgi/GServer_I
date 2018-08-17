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
