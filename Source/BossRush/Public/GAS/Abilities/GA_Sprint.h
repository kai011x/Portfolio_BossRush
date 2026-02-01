// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/CharacterBase.h"
#include "GA_Sprint.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_Sprint : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_Sprint();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;



protected:
	float WalkSpeed;
	float SprintSpeed;
	ACharacterBase* Character;
};
