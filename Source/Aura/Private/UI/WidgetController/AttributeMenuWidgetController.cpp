// Copyright Downwind Cloud Software, LLC


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	//for each attribute, we need to bind the onchange function
	for (auto& Pair : AS->TagsToAttributes)
	{
		//the mapping has the GameplayTag as the key and the function pointer to the attribute getter as the value.
		//we are going to use a lambda for each attribute to bind the delegate.
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				//find the AttributeInfo for the GameplayTag
				FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				//call the function pointer to get the attribute value
				//the function pointer is static, so you have to pass in the Attribute Set so it can access the attribute value.
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				//broadcast the attribute info
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo);
	//instead of setting up each atttribute, we use the map created in AuraAttributeSet that maps
	//the GameplayTag to the attribute getter function pointer.
	for (auto& Pair : AS->TagsToAttributes)
	{
		//find the AttributeInfo for the GameplayTag
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		//call the function pointer to get the attribute value
		//the function pointer is static, so you have to pass in the Attribute Set so it can access the attribute value.
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		//broadcast the attribute info
		AttributeInfoDelegate.Broadcast(Info);
	}
}


