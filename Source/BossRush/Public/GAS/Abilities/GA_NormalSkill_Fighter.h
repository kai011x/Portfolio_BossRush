// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Animation/AnimMontage.h"
#include "GA_NormalSkill_Fighter.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_NormalSkill_Fighter : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_NormalSkill_Fighter();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnMontageEnded();
	
	UFUNCTION()
	void OnLanded(EMovementMode NewMode);

	UAnimMontage* NormalSkillMontage;

};
