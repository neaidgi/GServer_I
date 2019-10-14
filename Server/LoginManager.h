#ifndef LOGIN_MANAGER_H
#define LOGIN_MANAGER_H

#include "User.h"

struct UserInfo
{
	char id[IDSIZE];
	char pw[IDSIZE];
};

/*
	[LoginManager]
	
	CreateInstance() - �Ŵ��� ����
	GetInstance() - �Ŵ��� ����������
	DestroyInstance() - �Ŵ��� ����
	
	InitializeManager() - �ʱ�ȭ
	EndManager() - ������
*/

// ����ȭ�� ����ϱ� ���� CMultiThreadSync ��ӹ���
class LoginManager : public CMultiThreadSync<LoginManager>
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

	// Ÿ��Ʋ
	RESULT TitleProcess(User* _user);
	//// INIR���¿��� �Ŵ� ����(�α���,ȸ������ �Ŵ�����,������)
	//RESULT logoutMenuChoice(User* user);
	//// ȸ�����Ի��¿� ���� ����(�ߺ�Ȯ��,ȸ������)
	//RESULT loginProcess(User* user);
};

#endif
