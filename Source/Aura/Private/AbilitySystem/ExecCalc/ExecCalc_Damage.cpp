// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

//not exposed to blueprints or unreal reflection system. Basically a native c++ struct, so not prefixed with U or F.
//static struct to create the Execution Capture Defintions one time and use them in all calculations
//probably should be its own file or base class file because we will use this in other calculations as well.
struct AuraDamageStatics
{
	//macro that declares a capture definition for the Armor attribute
	//creates FProperty* ArmorProperty and FGameplayEffectAttributeCaptureDefinition ArmorDef variables
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	//TODO: add critical hit chance, crit hit damage, and crit hit resistance
	//TODO: add resistances

	//this maps a gameplay tag to a capture definition for the attribute so we can look up a def by tag
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	AuraDamageStatics()
	{
		//macro that writes methods to set the defined variables with values passed in
		//requires class that has the attribute, the attribute named in the declare above, the enum value for EGameplayEffectAttributeCaptureSource, and snapshot boolean.
		//this creates functions to capture the Armor attribute from the target, without snapshotting it.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);

		//TODO: add the mappings for all the attributes
		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
	}
	
};
// Static instance of the struct to ensure it is created only once and can be accessed globally
static const AuraDamageStatics& GetAuraDamageStatics()
{
	static AuraDamageStatics Instance; // Create the static instance of the struct
	return Instance;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//add a list of attributes to capture for this calculation
	//uses the statics created above
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArmorPenetrationDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	// Here you would implement the logic to calculate damage based on attributes or other parameters.

	//Some information available from the params passed in
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	float SourceLevel = SourceCombatInterface->GetPlayerLevel();
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	float TargetLevel = TargetCombatInterface ? TargetCombatInterface->GetPlayerLevel() : 1.0f; // Default to 1 if no target combat interface

	//get parameters from the spec
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	//set those params to pass in to the evaluation of the attribute
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	//Access Curve Tables that adjust coefficients used in later calculations based on level of source and target.
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenCurve = CharacterClassInfo->DamageCoefficientsCurve->FindCurve(FName("ArmorPenetration"), TEXT(""), true);
	float ArmorPenCoefficient = ArmorPenCurve->Eval(SourceLevel); 
	const FRealCurve* ArmorCurve = CharacterClassInfo->DamageCoefficientsCurve->FindCurve(FName("EffectiveArmor"), TEXT(""), true);
	float ArmorCoefficient = ArmorCurve->Eval(TargetLevel);
	
	//Damage comes from a Set By Caller Magnitude using a tag of Damage. Get the value.
	//could be multiple damage types, so we can loop through them if needed.
	//need an array of damage types done if you want to have different actions or resistances per type.
	//right now, we just sum all damage types together to get a total damage value.
	float DamageValue = 0.0f;
	for (auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag& DamageTypeTag = Pair.Key;
		const FGameplayTag& ResistanceTag = Pair.Value;
		const FGameplayEffectAttributeCaptureDefinition ResistanceDef = GetAuraDamageStatics().TagsToCaptureDefs.FindChecked(ResistanceTag);
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false, 0.0f);
		//get value of the resistance
		float ResistanceValue = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceDef, EvaluateParams, ResistanceValue);
		//do we allow values outside 0 - 100?
		//calculate resistances and add to total damage
		//they need to be captured in the attribute set, so we can use the EvaluateParams to get the value.
		DamageValue += ((100.f-ResistanceValue)/100.f) * DamageTypeValue; //add the damage value of each damage type to the total damage value
	}
	

	
	//Capture Armor Value of Target
	float ArmorValue = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().ArmorDef, EvaluateParams,ArmorValue);
	//clamp or make sure the value falls within a range here if needed
	ArmorValue = FMath::Max<float>(0.0f, ArmorValue); //no negative armor

	//Capture Armor Penetration Value of Source
	float ArmorPenetrationValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().ArmorPenetrationDef, EvaluateParams, ArmorPenetrationValue);
	ArmorPenetrationValue = FMath::Max<float>(0.0f, ArmorPenetrationValue); //no negative armor penetration
	
	//Capture Block Chance of Target
	float BlockChanceValue = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().BlockChanceDef, EvaluateParams,BlockChanceValue);
	//clamp or make sure the value falls within a range here if needed
	BlockChanceValue = FMath::Max<float>(0.0f, BlockChanceValue); //no negative block chance
	
	/* Do whatever calculation you wanted to do here */
	//Is the incoming damage blocked?
	const bool bBlocked = FMath::RandRange(1, 100) < BlockChanceValue; // 1-100, so 1% chance to block if BlockChanceValue is 1.0f
	DamageValue = bBlocked ? (DamageValue * 0.5f) : DamageValue; //if blocked, halve the damage

	//set the fact that it is blocked in the custom context we created so we can use it later
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	//UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	//armor pen - negate a certain amount of armor based on armor penetration
	//apply armor penetration to armor value
	ArmorValue = FMath::Max<float>(0.0f, ArmorValue * (ArmorPenetrationValue * ArmorPenCoefficient / 100.0f)); // 25% of armor penetration is applied to armor value, so 100% armor penetration would negate all armor

	//apply adjusted armor value to damage
	DamageValue *= (100 - ArmorValue * ArmorCoefficient)/ 100.f; // 33% of armor value is applied to damage, so 100% armor would negate 33% of damage

	/* End Calculation */

	// Now we can set the output of the execution
	//add the output incomingdamage meta attribute value to the effect
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, DamageValue);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	// You can also add more modifiers to the output if needed
	
}
