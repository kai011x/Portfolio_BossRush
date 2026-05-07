// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "ArcherAttributeSet.generated.h"

/**
 * 
 */


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class BOSSRUSH_API UArcherAttributeSet : public UBasicAttributeSet
{
	GENERATED_BODY()
public:
	UArcherAttributeSet();
 
public:
	UPROPERTY(BlueprintReadOnly, Category = "AttackLevel", ReplicatedUsing = OnRep_AttackLevel)
	FGameplayAttributeData AttackLevel;
	ATTRIBUTE_ACCESSORS(UArcherAttributeSet, AttackLevel);

	UFUNCTION()
	void OnRep_AttackLevel(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UArcherAttributeSet, AttackLevel, OldValue);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


