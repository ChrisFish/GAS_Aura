// Copyright Downwind Cloud Software, LLC


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

//#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//blueprint library
	//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("UAuraProjectileSpell::ActivateAbility called from C++"), true, true, FLinearColor::Red, 5.f);
	//only call on server
	
	
	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	
	//get the combat interface from the actor avatar
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//TODO: Set Rotation
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f; //projectiles should not pitch
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		//spawn projectile class deferred to set more properties before actual commit
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//Give projectile a gameplay effect spec to apply damage
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle =  SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(), SourceASC->MakeEffectContext());
		if (SpecHandle.IsValid())
		{
			Projectile->DamageEffectSpecHandle = SpecHandle;
		}
		//finish spawning the projectile
		Projectile->FinishSpawning(SpawnTransform);
	}
}
