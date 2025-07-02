// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = Cast<AAuraPlayerState>(PC->PlayerState);
			UAbilitySystemComponent* ASC = PS ? PS->GetAbilitySystemComponent() : nullptr;
			UAttributeSet* AS = PS ? PS->GetAttributeSet() : nullptr;
			const FWidgetControllerParams WidgetParams(PC, PS, ASC, AS);
			return AuraHUD->GetOverlayWidgetController(WidgetParams);
		}
	}
	return nullptr;
}
