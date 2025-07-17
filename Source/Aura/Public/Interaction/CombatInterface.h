// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/* 
 * We need a struct that will map Montage Attack gameplay Tags to the Montage Asset.
 * Attack Abilities fire a montage. The montage mapping tells us where on the associated
 * mesh or actor the spot for the attack is, such as the hand or the weapon being used.
 * We can then use that spot to spawn projectiles or detect hits.
 * Each actor that implements the Combat Interface will tell us which montage to use, and
 * we can then use that montage to play the attack animation and figure out where damage/spawn
 * occurs.
 */

//this struct is the mapping of a montage to a gameplay tag
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()
	//the montage for the attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* MontageAsset = nullptr;
	//the tag that identifies the montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
};

/**
 * Combat Interface
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class.
public:
	//get the level of the current actor
	virtual int32 GetPlayerLevel();
	//get the socket location for combat spawns (such as projectiles)
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& TargetLocation);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactionMontage() const;

	virtual void Die()=0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();
};
