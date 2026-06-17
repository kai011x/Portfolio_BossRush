// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_DashAttack.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_DashAttack : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_DashAttack();
		
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
		
	FGameplayTag DashAttackTag;
	
	UFUNCTION()
	void OnMontageCompleted();
	
};
