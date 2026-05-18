// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ConditionAttack.generated.h"

/**
 * 적중 시 다음 콤보로 연계되는 특수 공격 어빌리티입니다.
 */
UCLASS()
class BOSSRUSH_API UGA_ConditionAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ConditionAttack();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	void PlayComboAction();

	UFUNCTION()
	void OnHitSuccessReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnCheckEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void OnMontageCompleted();

protected:
	int32 CurrentComboIndex;
	
	// 현재 재생 중인 몽타주에서 적중했는지 여부
	bool bHitSuccessful; 

	UPROPERTY()
	UDataTable* ComboDataTable;

	UPROPERTY()
	class UAbilityTask_PlayMontageAndWait* CurrentMontageTask;

	UPROPERTY()
	class UAbilityTask_WaitGameplayEvent* HitWaitTask;

	UPROPERTY()
	class UAbilityTask_WaitGameplayEvent* CheckWaitTask;
};
