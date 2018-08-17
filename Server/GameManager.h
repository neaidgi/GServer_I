#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "Global.h"
#include "IocpModel.h"

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