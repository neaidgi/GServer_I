#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "Global.h"

class Character
{
private:
	char character_name[20];		// ĳ���� �̸�
	int character_code;				// �ڵ�
	int	character_str;				// ��
	int	character_dex;				// ��
	int	character_int;				// ��
	int	character_health;			// ü��
	int	character_attackpoint;		// ���ݷ�
	int	character_defensepoint;		// ����
	int	character_speed;			// �̼�
	int	character_growthhealth;		// ���� ü��
	int	character_growthstr;		// ���� ��
	int	character_growthdex;		// ���� ��
	int	character_growthint;		// ���� ��
};

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