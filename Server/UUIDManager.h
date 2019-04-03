#ifndef UUID_MANAGER_H
#define UUID_MANAGER_H

#include "User.h"
#include "CriticalSectionManager.h"
#include <list>

class UUIDManager
{
private:
	static UUIDManager* Instance;

	UUIDManager();
	~UUIDManager();

	int partyroomnum_default;

	// 파티번호
	std::list<int> partyroomnum;
	std::list<int>::iterator save;

	// 파티번호있는지
	bool isPartyRoomNum(int _partyroomnum);

public:
	static void CreateInstance();
	static UUIDManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// 캐릭터코드 만드는함수
	void UUID_CharacterUniqCode(User* _user, char * _codebuf, int _jobcode, int _len, char* _nick);
	// 파티번호 만드는 함수
	int UUID_PartyRoomNum();
};

#endif
