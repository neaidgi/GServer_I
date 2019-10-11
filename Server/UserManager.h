#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include "CriticalSection.h"

/*
	[UserManager]

	CreateInstance() - �Ŵ��� ����
	GetInstance() - �Ŵ��� ����������
	DestroyInstance() - �Ŵ��� ����

	InitializeManager() - �ʱ�ȭ
	EndManager() - ������
*/

// ����ȭ�� ����ϱ� ���� CMultiThreadSync ��ӹ���
class UserManager : public CMultiThreadSync<UserManager>
{
private:
	std::list<User*> ConnectUserlist;		// ������ ���� ����Ʈ
	std::list<User*>::iterator save;		// �˻��� ���� ������ ����
	static UserManager* Instance;

	UserManager();
	~UserManager();

public:
	static void CreateInstance();
	static UserManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// ���� �߰�
	User* addUser(SOCKET _sock);
	// ���� ����
	void removeUser(User* _user);
	// ���� �˻�(����)
	User* getUser(SOCKET _sock);
	// ���� �˻�(���̵�)
	User* getUser(char* _id);
	// ���� �ڵ� �˻�(�ڵ�)
	User* getUserCode(char* _code);
	// ���� �ڵ� �˻�(�ڵ�)
	User* getUserCode(const char* _code);
	// ������ �ִ��� Ȯ��
	bool isUser(User* _user);

	// �˻� ����
	void startSearch();
	// ���� �˻�
	bool searchData(User*& _user);
};

#endif
