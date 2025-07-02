// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura Gameplay Tags
 *
 * Singleton struct for managing gameplay tags in the Aura project.
 * 
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get()
	{
		return Instance;
	}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Secondary_Armor;
	
protected:
private:
	static FAuraGameplayTags Instance;
};