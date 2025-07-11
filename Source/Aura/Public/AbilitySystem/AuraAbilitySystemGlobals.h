// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 *  Override the globals to use any custom GAS classes that we want.
 *  Tell Unreal to use this class by editing the DefaultGame.ini config file.
 *
 *  [/Script/GameplayAbilities.AbilitySystemGlobals]
 *  +AbilitySystemGlobalsClassName=/Script/Aura.AuraAbilitySystemGlobals
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
