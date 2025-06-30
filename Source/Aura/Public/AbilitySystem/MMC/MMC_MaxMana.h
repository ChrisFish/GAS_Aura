// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/*
 *  Custom Gameplay Mod Magnitude Calculation for calculating the maximum mana of a character based on their Intelligence attribute.
 *  Can't be an attribute because we are going to include Level in the calculation, and is not an attribute.
 *  Level comes from the Combat Interface of the object that generated the effect spec.
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntDef;
};
