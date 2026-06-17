// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_NormalSkill.generated.h"

/**
 * 일반 스킬의 공통 기능을 담당하는 기반 클래스
 */
UCLASS()
class BOSSRUSH_API UGA_NormalSkill : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_NormalSkill();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	/** 자식 클래스에서 실제 스킬 로직을 구현할 가상 함수 */
	virtual void ExecuteSkillLogic();

	/** 몽타주 종료 시 호출될 공통 콜백 */
	UFUNCTION()
	virtual void OnSkillMontageEnded();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FName SkillRowName = TEXT("NormalSkill");

	UPROPERTY()
	class UAnimMontage* SkillMontage;
};
