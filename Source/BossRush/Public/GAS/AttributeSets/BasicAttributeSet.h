// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BasicAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class BOSSRUSH_API UBasicAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UBasicAttributeSet();


	//Health Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxHealth);

	//Groggy Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxGroggy)
	FGameplayAttributeData MaxGroggy;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxGroggy);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Groggy)
	FGameplayAttributeData Groggy;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Groggy);

	//Stamina Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxStamina);

	//Resource Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Resource)
	FGameplayAttributeData Resource;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Resource);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxResource)
	FGameplayAttributeData MaxResource;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxResource);

	UPROPERTY(BlueprintReadOnly, Category = "DashCount", ReplicatedUsing = OnRep_DashCount)
	FGameplayAttributeData DashCount;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, DashCount);

	UPROPERTY(BlueprintReadOnly, Category = "DashCount", ReplicatedUsing = OnRep_MaxDashCount)
	FGameplayAttributeData MaxDashCount;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, MaxDashCount);


	UPROPERTY(BlueprintReadOnly, Category = "Defense", ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Defense);

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Attack);
	
	UPROPERTY(BlueprintReadOnly, Category = "Critical", ReplicatedUsing = OnRep_Critical)
	FGameplayAttributeData Critical;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Critical);
	
	// Meta Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, Damage);

	UPROPERTY(BlueprintReadOnly, Category = "Speed", ReplicatedUsing = OnRep_RunSpeed)
	FGameplayAttributeData RunSpeed;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, RunSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Speed", ReplicatedUsing = OnRep_SprintSpeed)
	FGameplayAttributeData SprintSpeed;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, SprintSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Dash", ReplicatedUsing = OnRep_DashStrength)
	FGameplayAttributeData DashStrength;
	ATTRIBUTE_ACCESSORS(UBasicAttributeSet, DashStrength);


public:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Health, OldValue);
	}
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxHealth, OldValue);
	}
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Stamina, OldValue);
	}
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxStamina, OldValue);
	}
	
	UFUNCTION()
	void OnRep_Resource(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Resource, OldValue);
	}
	
	UFUNCTION()
	void OnRep_MaxResource(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxResource, OldValue);
	}

	UFUNCTION()
	void OnRep_DashCount(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, DashCount, OldValue); }

	UFUNCTION()
	void OnRep_MaxDashCount(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxDashCount, OldValue); }

	// [�߰�] ��Ÿ ���ȿ� OnRep
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Defense, OldValue); }

	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Attack, OldValue); }

	UFUNCTION()
	void OnRep_RunSpeed(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, RunSpeed, OldValue); }

	UFUNCTION()
	void OnRep_SprintSpeed(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, SprintSpeed, OldValue); }

	UFUNCTION()
	void OnRep_DashStrength(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, DashStrength, OldValue); }

	UFUNCTION()
	void OnRep_Critical(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Critical, OldValue); }

	UFUNCTION()
	void OnRep_MaxGroggy(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, MaxGroggy, OldValue); }

	UFUNCTION()
	void OnRep_Groggy(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet, Groggy, OldValue); }


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

};
