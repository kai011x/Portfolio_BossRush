// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/Boss/GA_AttackPatternBase.h"
#include "GA_WukongAttack.generated.h"

/**
 * 오공 보스의 공격 패턴 어빌리티
 */
UCLASS()
class BOSSRUSH_API UGA_WukongAttack : public UGA_AttackPatternBase
{
	GENERATED_BODY()

public:
	UGA_WukongAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnMontageFinished();
};
