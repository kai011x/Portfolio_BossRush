// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_UltraSkill.h"
#include "GA_UltraSkill_Fighter.generated.h"

/**
 * Fighter 전용 궁극기 - 애니메이션 중심
 */
UCLASS()
class BOSSRUSH_API UGA_UltraSkill_Fighter : public UGA_UltraSkill
{
	GENERATED_BODY()

public:
	UGA_UltraSkill_Fighter();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	/** 실행할 궁극기 몽타주 */
	UPROPERTY(EditAnywhere, Category = "Skill|Animation")
	TObjectPtr<UAnimMontage> UltraSkillMontage;

	/** 몽타주 종료 시 호출되는 콜백 */
	UFUNCTION()
	void OnMontageEnded();
};
