// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataUnderMouseSignature, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 *  Custom Async Task for collecting target data under the mouse cursor.
 *  Requires UAbilitySystemGlobals::Get().InitGlobalData(); in the AssetManager StartInitialLoading() method since
 *  we override with our own Asset Manager.
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/* Factory type pattern to return an instance of this task exposed to Blueprints.
	 * OwningAbility is the Gameplay Ability that owns this task.
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta=(DisplayName="Target Data Under Mouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UTargetDataUnderMouse* TargetDataUnderMouse(UGameplayAbility* OwningAbility);

	// Event for when valid target data is found under the mouse cursor.
	UPROPERTY(BlueprintAssignable)
	FTargetDataUnderMouseSignature OnValidData;

private:
	/* override the active function (ie Execute()) */
	virtual void Activate() override;

	//if we are locally controlled in multiplayer, have to send the cursor data to the server.
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
