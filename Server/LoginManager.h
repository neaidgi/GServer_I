#ifndef LOGIN_MANAGER_H
#define LOGIN_MANAGER_H

#include "User.h"

const struct
{
	enum {
		LOGIN = 1,
		JOIN = 2,
		EXIT
	};
} LogoutMenu = {};
const struct
{
	enum {
		TENDER = 1,
		LEAVE,
		LOGOUT,
		EXIT
	};
} LoginMenu = {};

struct UserInfo
{
	char id[IDSIZE];
	char pw[IDSIZE];
};

class LoginManager
{
private:
	static LoginManager* Instance;
	UserInfo* userList[USERCOUNT];
	int userCount;

	LoginManager();
	~LoginManager();
	const UserInfo* getUser(const char* id);

	// ID 중복체크
	void reqIdOverlapCheck(User* user, char* _buf);
	// 회원가입 결과
	void reqJoin(User* user, char* _buf);
	// 로그인 결과
	bool reqLogin(User* user, char* _buf);

public:
	static void CreateInstance();
	static LoginManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// INIR상태에서 매뉴 선택(로그인,회원가입,나가기)
	RESULT logoutMenuChoice(User* user);
	// 
	RESULT loginProcess(User* user);
	// 로그인상태에서 매뉴 선택()
	RESULT loginMenuChoice(User* user);
};

#endif
