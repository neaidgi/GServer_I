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

	// ��Ƽ��ȣ
	std::list<int> partyroomnum;
	std::list<int>::iterator save;

	// ��Ƽ��ȣ�ִ���
	bool isPartyRoomNum(int _partyroomnum);

public:
	static void CreateInstance();
	static UUIDManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	// ĳ�����ڵ� ������Լ�
	void UUID_CharacterUniqCode(User* _user, char * _codebuf, int _jobcode, int _len, char* _nick);
	// ��Ƽ��ȣ ����� �Լ�
	int UUID_PartyRoomNum();
};

#endif
