#include "Character.h"

Character & Character::operator=(const Character & rhs)
{
	character_jobcode = rhs.character_jobcode;
	character_code = rhs.character_code;
	character_str = rhs.character_str;
	character_dex = rhs.character_dex;
	character_int = rhs.character_int;
	character_health = rhs.character_health;
	character_mana = rhs.character_mana;
	character_attackpoint = rhs.character_attackpoint;
	character_defensepoint = rhs.character_defensepoint;
	character_speed = rhs.character_speed;
	character_growhealth = rhs.character_growhealth;
	character_growmana = rhs.character_growmana;
	character_growstr = rhs.character_growstr;
	character_growdex = rhs.character_growdex;
	character_growint = rhs.character_growint;
	character_jumpstate = rhs.character_jumpstate;

	return *this;
}
