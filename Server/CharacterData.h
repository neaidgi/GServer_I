#ifndef _CHARACTERDATA_H_
#define _CHARACTERDATA_H_

#include "Character.h"
#include "Global.h"

class CharacterData {
private:
	// 캐릭터 기본 설계도
	Character * CharacterOrigin[MAXCHARACTERORIGIN];
	bool isLoad;
public:
	CharacterData();
	~CharacterData();

	void SetCharacterOrigin(Character* _origin[]);

	const Character* GetTanker() { return CharacterOrigin[0]; }
	const Character* GetWarrior() { return CharacterOrigin[1]; }
	const Character* GetMagician() { return CharacterOrigin[2]; }
	const Character* GetGunner() { return CharacterOrigin[3]; }
};

#endif