#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "User.h"
#include "CriticalSection.h"

/*
	[UserManager]

	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제

	InitializeManager() - 초기화
	EndManager() - 마무리
*/

// 동기화를 사용하기 위해 CMultiThreadSync 상속받음
class UserManager : public CMultiThreadSync<UserManager>
{
private:
	std::list<User*> ConnectUserlist;		// 접속한 유저 리스트
	std::list<User*>::iterator save;		// 검색용 유저 저장할 변수
	static UserManager* Instance;

	UserManager();
	~UserManager();

public:
	static void CreateInstance();
	static UserManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// 유저 추가
	User* addUser(SOCKET _sock);
	// 유저 삭제
	void removeUser(User* _user);
	// 유저 검색(소켓)
	User* getUser(SOCKET _sock);
	// 유저 검색(아이디)
	User* getUser(char* _id);
	// 유저 코드 검색(코드)
	User* getUserCode(char* _code);
	// 유저 코드 검색(코드)
	User* getUserCode(const char* _code);
	// 유저가 있는지 확인
	bool isUser(User* _user);

	// 검색 시작
	void startSearch();
	// 유저 검색
	bool searchData(User*& _user);
};

#endif
