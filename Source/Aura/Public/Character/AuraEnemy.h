// Copyright Downwind Cloud Software, LLC

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"
/**
 * 
 */
class UBehaviorTree;
class AAuraAIController;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	virtual void PossessedBy(AController* NewController) override;

	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnhightlightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* Target) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/* End Enemy Interface */
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	
	/* End Combat Interface */

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float BaseMoveSpeed = 250.f;

	UPROPERTY(BlueprintReadWrite, Category="Combat")
	TObjectPtr<AActor> CombatTarget;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	
private:
	virtual void InitAbilityActorInfo() override;

	 


	
	

};
