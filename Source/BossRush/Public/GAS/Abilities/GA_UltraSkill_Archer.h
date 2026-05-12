// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_UltraSkill.h"
#include "GA_UltraSkill_Archer.generated.h"

/**
 * Archer 전용 궁극기
 */
UCLASS()
class BOSSRUSH_API UGA_UltraSkill_Archer : public UGA_UltraSkill
{
	GENERATED_BODY()

public:
	UGA_UltraSkill_Archer();

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	virtual void ExecuteSkillLogic() override;
};
