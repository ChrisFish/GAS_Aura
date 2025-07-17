// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FScalableFloat> DamageType : DamageTypes)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType.Key, DamageType.Value.GetValueAtLevel(1.f));
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontage(const TArray<FTaggedMontage>& AttackMontages) const
{
	//make sure the array is not empty
	if (AttackMontages.Num() == 0) {return FTaggedMontage();}
	//get a random index
	const int32 RandomIndex = FMath::RandRange(0, AttackMontages.Num() - 1);
	return AttackMontages[RandomIndex];
}
