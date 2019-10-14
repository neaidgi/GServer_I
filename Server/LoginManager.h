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
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

// 동기화를 사용하기 위해 CMultiThreadSync 상속받음
class LoginManager : public CMultiThreadSync<LoginManager>
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

	// 타이틀
	RESULT TitleProcess(User* _user);
	//// INIR상태에서 매뉴 선택(로그인,회원가입 매뉴선택,나가기)
	//RESULT logoutMenuChoice(User* user);
	//// 회원가입상태에 따른 선택(중복확인,회원가입)
	//RESULT loginProcess(User* user);
};

#endif
