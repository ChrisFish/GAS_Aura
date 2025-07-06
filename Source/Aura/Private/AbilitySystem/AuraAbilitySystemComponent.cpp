// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//character and/or enemy have set their ability actor info and notify here.
	//set delegates
	//change to client rpc.
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

	//test native tags
	//const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Orange,FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> ThisAbility : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(ThisAbility, 1.0f);
		if (const UAuraGameplayAbility* ThisAuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			//if the ability has a startup input tag, add it to the ability spec
			if (ThisAuraAbility->StartupInputTag.IsValid())
			{
				AbilitySpec.GetDynamicSpecSourceTags().AddTag(ThisAuraAbility->StartupInputTag);
				GiveAbility(AbilitySpec);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& ThisAbilitySpec : GetActivatableAbilities())
	{
		if (ThisAbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(ThisAbilitySpec);
			if (!ThisAbilitySpec.IsActive())
			{
				TryActivateAbility(ThisAbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	//gets called when a gameplay effect is applied to this ability system component

	//ability system component will broadcast changes via gameplay tags to widget controller or other systems
	//use the effectspec to get the tags
	FGameplayTagContainer TagContainer;
	SpecApplied.GetAllAssetTags(TagContainer);
	//broadcast tags to widget controller
	EffectAssetTags.Broadcast(TagContainer);

}
