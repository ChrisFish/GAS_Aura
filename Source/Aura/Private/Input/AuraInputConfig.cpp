// Copyright Downwind Cloud Software, LLC


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::GetInputActionForGameplayTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Input tag [%s] not found in [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}