// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(const ECharacterClass CharacterClass)
{
	return CharacterClassInfoMap.FindChecked(CharacterClass);
}
