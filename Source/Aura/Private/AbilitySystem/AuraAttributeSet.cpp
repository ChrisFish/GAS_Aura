// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Net/UnrealNetwork.h"


UAuraAttributeSet::UAuraAttributeSet()
{
	//we need to set up the map for each attribute here one time.
	//more boilerplate, but if you add new attributes, you only need to work in this file.
	/* Primary Attributes */
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Primary_Strength, &UAuraAttributeSet::GetStrengthAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Primary_Intelligence, &UAuraAttributeSet::GetIntelligenceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Primary_Resilience, &UAuraAttributeSet::GetResilienceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Primary_Vigor, &UAuraAttributeSet::GetVigorAttribute);
	/* Secondary Attributes */
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_Armor, &UAuraAttributeSet::GetArmorAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_ArmorPenetration, &UAuraAttributeSet::GetArmorPenetrationAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_BlockChance, &UAuraAttributeSet::GetBlockChanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitChance, &UAuraAttributeSet::GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitDamage, &UAuraAttributeSet::GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitResistance, &UAuraAttributeSet::GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_HealthRegeneration, &UAuraAttributeSet::GetHealthRegenerationAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_ManaRegeneration, &UAuraAttributeSet::GetManaRegenerationAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_MaxHealth, &UAuraAttributeSet::GetMaxHealthAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Secondary_MaxMana, &UAuraAttributeSet::GetMaxManaAttribute);
	/* Resistance Attributes */
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Resistance_Arcane, &UAuraAttributeSet::GetArcaneResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Resistance_Fire, &UAuraAttributeSet::GetFireResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Resistance_Lightning, &UAuraAttributeSet::GetLightningResistanceAttribute);
	TagsToAttributes.Add(FAuraGameplayTags::Get().Attributes_Resistance_Physical, &UAuraAttributeSet::GetPhysicalResistanceAttribute);
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/* Primary Attributes */
	//strength -> Increase Physical damage
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	//intelligence -> Increase Magical damage
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	//resilience -> Increase Armor and Armor Penetration
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	//vigor -> Increase Health and Health Regeneration
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	/* Secondary Attributes */
	//Armor - calculated from Resilience -> Reduces Damage taken and improves block chance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	//Armor Penetration - calculated from Resilience -> Ignores enemy armor, increase crit hit chance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	//Block Chance - calculated from Armor -> Chance to block incoming damage by half
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	//Critical Hit Chance - calculated from Armor Penetration -> Chance to deal double damage on hit + critical hit damage
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	//Critical Hit Damage - calculated from Armor Penetration -> Amount of additional damage dealt on a critical hit
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	//Critical Hit Resistance - calculated from Armor -> Reduces chance of being critically hit by enemies
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	//Health Regeneration - calculated from Vigor -> Amount of health restored every 1 second
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	//Mana Regeneration - calculated from Intelligence -> Amount of mana restored every 1 second
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	//Max Health - calculated from Vigor -> Maximum amount of health the character can have
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	//Max Mana - calculated from Intelligence -> Maximum amount of mana the character can have
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	/* Vital Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	/* Resistance Attributes */
	//Fire
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	//Lightning
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	//Arcane
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	//Physical
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	
}
//Clamp attributes
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		const float OldHealth = Health.GetCurrentValue();
		const float NewHealth = FMath::Clamp(NewValue, 0.0f, MaxHealth.GetCurrentValue());
		NewValue = NewHealth;
	}
	else if (Attribute == GetManaAttribute())
	{
		const float OldMana = Mana.GetCurrentValue();
		const float NewMana = FMath::Clamp(NewValue, 0.0f, MaxMana.GetCurrentValue());
		NewValue = NewMana;
	}
	//clamp resistances to no more than 100%? Allow negative to show susceptibility?
}
void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& Props) const
{
	// Source = causer of effect. Target = Target of effect (owner of this attribute set)

	//try to get the source avatar actor and controller. Might be a Pawn.
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	const UAbilitySystemComponent* SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (SourceASC && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = SourceASC->GetAvatarActor();
		Props.SourceController = SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceAvatarActor != nullptr && Props.SourceController == nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController != nullptr)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	//target info
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);

		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}
void UAuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	//generic struct of common effect properties we may need access to later. For testing, debugging, etc.
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	//clamping health and mana between 0 and max values
	if(Data.EvaluatedData.Attribute == GetHealthAttribute()){
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Changed health on %s, Health %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute()){
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);
		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, MaxHealth.GetCurrentValue()));
			const bool bFatal = NewHealth <= 0.0f;
			if (bFatal)
			{
				ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
				if (CombatInterface)
				{
					CombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
		}
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldMana);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldValue);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldValue);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldValue);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldValue);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxMana);
}


