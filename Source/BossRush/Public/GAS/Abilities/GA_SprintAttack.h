// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_SprintAttack.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_SprintAttack : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_SprintAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	FGameplayTag SprintAttackTag;

	UFUNCTION()
	void OnMontageCompleted();
};
