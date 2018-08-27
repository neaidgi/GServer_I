#ifndef _CHARACTER_MANAGER_H_
#define _CHARACTER_MANAGER_H_

#include "Global.h"

class Character
{
private:
	char character_name[20];		// 캐릭터 이름
	int character_code;				// 코드
	int	character_str;				// 힘
	int	character_dex;				// 민
	int	character_int;				// 지
	int	character_health;			// 체력
	int	character_attackpoint;		// 공격력
	int	character_defensepoint;		// 방어력
	int	character_speed;			// 이속
	int	character_growthhealth;		// 성장 체력
	int	character_growthstr;		// 성장 힘
	int	character_growthdex;		// 성장 민
	int	character_growthint;		// 성장 지
};

class CharacterManager 
{
private:
	static CharacterManager* Instance;
	// 직업 3개 list로 관리

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