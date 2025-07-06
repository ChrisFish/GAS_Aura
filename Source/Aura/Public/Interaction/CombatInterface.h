// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class.
public:
	//get the level of the current actor
	virtual int32 GetPlayerLevel();
	//get the socket location for combat spawns (such as projectiles)
	virtual FVector GetCombatSocketLocation();
};
