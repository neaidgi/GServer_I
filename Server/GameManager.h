#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "Global.h"
#include "IocpModel.h"

/*
	[GameManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

class GameManager : public IocpModel 
{
private:
	static GameManager* Instance;

	GameManager();
	~GameManager();
public:
	static void CreateInstance();
	static GameManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	bool acceptProcess(User* _user);
	bool recvProcess(User* _user);
	bool sendProcess(User* _user);

};

#endif