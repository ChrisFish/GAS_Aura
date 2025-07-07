// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::TargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* Instance = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	if (Instance)
	{
		// Initialize any properties or perform setup here if needed
	}
	return Instance;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//local execution of an ability, so we can get the cursor data.
		SendMouseCursorData();
	}
	else
	{
		//running replicated ability on the server in this case.
		//these are the keys used for mapping the ability to its replicated data sent up.
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		//need to use a combination of Ability SpecHandle and PredictionKey to get the data from the client.
		//this will get a delegate to listen for the data from the client.
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		//what if the data got here before the server activated the ability.
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		//data not here yet?
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	
	
	// End the task after broadcasting the event.
	//EndTask();
}

/* Get the cursor data locally and then package it up for sending to server. Include prediction key */
void UTargetDataUnderMouse::SendMouseCursorData()
{
	//set a window start point for prediction to sync client and server
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	//get the player controller from the owning ability
	APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	// get the hit result under the mouse cursor
	PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	
	//create a hit result targetdata data object to send to the server
	//these are builtin gas structs for handling target data sending
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = HitResult;
	DataHandle.Add(Data);

	//actually sending the data passing the prediction key and the data handle.
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	//check to see if the ability is still active before broadcasting the event.
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	//called on server when targetdata struct is replicated from the client to the server. ability is ready to activate on server.

	//consume the record from the mapping because we have everything we need now.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnValidData.Broadcast(DataHandle);
	}
}


