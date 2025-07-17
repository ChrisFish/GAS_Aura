// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystem|GameplayMechanics")
	void CauseDamage(AActor* TargetActor);
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	//utility function to take a set of attack montages and return a random one
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystem|GameplayMechanics")
	FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& AttackMontages) const;
};
