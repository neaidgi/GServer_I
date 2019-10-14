#ifndef UUID_MANAGER_H
#define UUID_MANAGER_H

#include "User.h"
#include "CriticalSection.h"
#include <list>
/*
	[UUIDManager]
	
	CreateInstance() - 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

// 동기화를 사용하기 위해 CMultiThreadSync 상속받음
class UUIDManager : public CMultiThreadSync<UUIDManager>
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
