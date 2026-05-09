// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ArcherAimAttack.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_ArcherAimAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ArcherAimAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION()
	void OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& DataHandle);

	UFUNCTION()
	void OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& DataHandle);
};
