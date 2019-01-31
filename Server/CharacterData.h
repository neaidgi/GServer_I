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

	void GetTanker(Character* _t) { memcpy(_t, CharacterOrigin[0], sizeof(Character)); }
	void GetWarrior(Character* _w) { memcpy(_w, CharacterOrigin[1], sizeof(Character)); }
	void GetMagician(Character* _m) { memcpy(_m, CharacterOrigin[2], sizeof(Character)); }
	void GetGunner(Character* _m) { memcpy(_m, CharacterOrigin[3], sizeof(Character)); }
};

#endif