// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_UltraSkill_Fighter.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_UltraSkill_Fighter : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGA_UltraSkill_Fighter();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	void PlayActionStart();

	void PlayStartStep();

	void StartInputListener();

	void PlayComboStep();

	void PlayLoopStep();

	void PlayFinishStep();

	UFUNCTION()
	void OnInputPressed(float TimeWaited);

	UFUNCTION()
	void OnStepCompleted();

	UFUNCTION()
	void OnAbilityEnded();
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "UltraSkill")
	UAnimMontage* StartLoopMontage;

	UPROPERTY(BlueprintReadOnly, Category = "UltraSkill")
	UAnimMontage* FinishMontage;

private:
	bool bInputReceived;

	int32 MaxLoopCount = 10;
	int32 CurrentLoopCount = 0;
};
