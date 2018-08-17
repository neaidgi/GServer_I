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

	// ID �ߺ�üũ
	void reqIdOverlapCheck(User* user, char* _buf);
	// ȸ������ ���
	void reqJoin(User* user, char* _buf);
	// �α��� ���
	bool reqLogin(User* user, char* _buf);

public:
	static void CreateInstance();
	static LoginManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// INIR���¿��� �Ŵ� ����(�α���,ȸ������,������)
	RESULT logoutMenuChoice(User* user);
	// 
	RESULT loginProcess(User* user);
	// �α��λ��¿��� �Ŵ� ����()
	RESULT loginMenuChoice(User* user);
};

#endif
