#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include "CriticalSectionManager.h"

class UserManager
{
private:
	//User * list[CLIENTCOUNT];				// std ����Ʈ�� ��ü ����
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

	User* addUser(SOCKET _sock);
	void removeUser(User* _user);
	User* getUser(SOCKET _sock);
	User* getUser(char* _id);
	User* getUserCode(char* _code);
	User* getUserCode(const char* _code);
	bool isUser(User* _user);

	void startSearch();						// ����Ʈ
	bool searchData(User*& _user);			// ����Ʈ
};

#endif
