#ifndef ENCRYPT_MANAGER_H
#define ENCRYPT_MANAGER_H

#include <Windows.h>
/*
[EncryptManager]

	CreateInstance(int key) - 특정 키 값으로 매니저 생성
	GetInstance() - 매니저 접근지정자
	DestroyInstance() - 매니저 삭제
	
	InitializeManager() - 초기화
	EndManager() - 마무리
*/

class EncryptManager
{
private:
	char key[4];
	static EncryptManager* Instance;

	EncryptManager(int key);
	~EncryptManager();

public:
	static void CreateInstance(int key);
	static EncryptManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void encoding(char *packet, int size);
	void decoding(char *packet, int size);
};
#endif