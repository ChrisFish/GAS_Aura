// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraInputConfig;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
class UAuraAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const struct FInputActionValue& InputActionValue);
	void CursorTrace();
	FHitResult CursorHit;
	
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;
	UAuraAbilitySystemComponent* GetAuraAbilitySystemComponent();

	/* Click to move variables */
	FVector CachedDestination = FVector::ZeroVector; //where the user clicked
	float FollowTime = 0.f; //how long the user has been following the click
	float ShortPressThreshold = 0.5f; //how long the user has to hold the click to be considered a short press
	bool bAutoRunning = false; // whether the player is auto-running or not
	bool bTargeting = false; // whether the player is targeting an enemy or not
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f; // how close the player has to be to the destination to stop auto-running
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; //Autorun pathing spline for smoothing the run path

	void AutoRun();
};
