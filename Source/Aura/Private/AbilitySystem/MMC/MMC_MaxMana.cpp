// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

class ICombatInterface;

UMMC_MaxMana::UMMC_MaxMana()
{
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//get a parameter collection and apply the tags to the calculations
	//these will be parameters we will give to the request for current Int in this case
	//allows us to ask for the current Int with all the known parameters Int needs in order to
	//give us the correct magnitude.
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	//what is the current value of Int, taking into account all the modifiers and tags
	float IntValue = 0.0f;
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluateParams, IntValue);
	// Ensure Int is not negative
	IntValue = FMath::Max<float>(IntValue, 0.0f);

	//Now we need the Level from the spec using the interface.
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 CurrentLevel = CombatInterface ? CombatInterface->GetPlayerLevel() : 1;
	
	//calculate the base magnitude of the effect
	//this is where you can make complex calculations you can't do in the effect editor..
	return 80.f + (IntValue * 2.5f) + (CurrentLevel * 10.0f);
}
