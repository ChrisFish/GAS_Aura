#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"
/*
 * This will be a custom gameplay effect context for the project. It will be set
 * as the default context for all gameplay effects. This will allow us to send data
 * across the network with gameplay effects that include custom data.
 * An example of extending this data would be to include a boolean for a critical hit
 * since that will be calculated in an execution calculation.
 *
 * You have to set the project to use this custom context in the project settings.
 *
 * This is a serialized object, so we have to implement some methods to serialize and deserialize
 * from the base class.
 * It also uses bit shifting to store bool values and to flag whether other data (such as objects)
 * should be included in the serialization. The base class implements many functions to serialize
 * different data types.
 *
 * Multiplayer RPG with GAS, lecture 150 (Advanced Damage section) for more information on this topic.
 */


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	//here are our custom values we want to add to the context
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	//required override for the reflection system to create this struct. 
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	//required override to serialize this struct
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
	//required override to duplicate the instance
	virtual FAuraGameplayEffectContext* Duplicate() const override;
	
protected:
	UPROPERTY()
	bool bIsBlockedHit = false;
	UPROPERTY()
	bool bIsCriticalHit = false;
};

// This is necessary to allow the reflection system to know what it can do with the defined struct
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext>: public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,	// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};