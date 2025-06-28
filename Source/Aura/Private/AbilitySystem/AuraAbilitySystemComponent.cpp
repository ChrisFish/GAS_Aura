// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	//character and/or enemy have set their ability actor info and notify here.
	//set delegates
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
	
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	//gets called when a gameplay effect is applied to this ability system component

	//ability system component will broadcast changes via gameplay tags to widget controller or other systems
	//use the effectspec to get the tags
	FGameplayTagContainer TagContainer;
	SpecApplied.GetAllAssetTags(TagContainer);
	//broadcast tags
	for (const FGameplayTag& Tag : TagContainer)
	{
		//broadcast tag to widget controller
		const FString msg = FString::Printf(TEXT("Tag Applied: %s"), *Tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.0f, FColor::Blue, msg);
	}
	
}
