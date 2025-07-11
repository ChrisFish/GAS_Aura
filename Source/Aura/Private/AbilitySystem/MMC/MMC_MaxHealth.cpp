// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	//definition of the attribute capture(s) we want to use for this calculation
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false; // Snapshot at time of calculation or time of effect application. This is false because vigor does not change often.

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//get a parameter collection and apply the tags to the calculations
	//these will be parameters we will give to the request for current Vigor in this case
	//allows us to ask for the current Vigor with all the known parameters Vigor needs in order to
	//give us the correct magnitude.
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	//what is the current value of Vigor, taking into account all the modifiers and tags
	float VigorValue = 0.0f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParams, VigorValue);
	// Ensure Vigor is not negative
	VigorValue = FMath::Max<float>(VigorValue, 0.0f);

	//Now we need the Level from the spec using the interface.
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 CurrentLevel = CombatInterface ? CombatInterface->GetPlayerLevel() : 1;
	
	//calculate the base magnitude of the effect
	//this is where you can make complex calculations you can't do in the effect editor..
	return 80.f + (VigorValue * 2.5f) + (CurrentLevel * 10.0f);
}
