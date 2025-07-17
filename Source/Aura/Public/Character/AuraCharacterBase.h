// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAuraAttributeSet;
class UAuraAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	virtual UAuraAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAuraAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Highlighting
	UFUNCTION(BlueprintCallable, Category="Aura|Character")
	virtual void HighlightActor() PURE_VIRTUAL(AAuraCharacterBase::HighlightActor, );
	UFUNCTION(BlueprintCallable, Category="Aura|Character")
	virtual void UnhighlightActor() PURE_VIRTUAL(AAuraCharacterBase::UnhighlightActor, );

	
	/* Combat Interface */
	virtual UAnimMontage* GetHitReactionMontage_Implementation() const override;
	virtual void Die() override;
	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandTipSocketName;
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandTipSocketName;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;

	bool bIsDead = false;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;

	//Here are the montages that can be used for attacks by this actor.
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	/* End Combat Interface */

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//where is the socket location for casting projectiles
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesEffectClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesEffectClass;
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();
	
private:
	virtual void InitAbilityActorInfo();

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactionMontage;
	
	UPROPERTY(EditAnywhere, Category="GAS|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
