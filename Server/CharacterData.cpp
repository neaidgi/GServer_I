#include "CharacterData.h"

CharacterData::CharacterData()
{
	memset(CharacterOrigin, 0, sizeof(CharacterOrigin));
	isLoad = false;
}

CharacterData::~CharacterData()
{
	if (isLoad)
	{
		for (int i = 0; i < MAXCHARACTERORIGIN; i++)
		{
			delete CharacterOrigin[i];
		}
	}
}

void CharacterData::SetCharacterOrigin(Character* _origin[])
{
	for (int i = 0; i < MAXCHARACTERORIGIN; i++)
	{
		CharacterOrigin[i] = _origin[i];
	}
	isLoad = true;
}
