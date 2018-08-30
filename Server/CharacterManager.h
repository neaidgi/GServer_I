#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "Global.h"


class CharacterManager 
{
private:
	static CharacterManager* Instance;
	// ���� 3�� list�� ����

	CharacterManager();
	~CharacterManager();
public:
	static void CreateInstance();
	static CharacterManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

};

#endif