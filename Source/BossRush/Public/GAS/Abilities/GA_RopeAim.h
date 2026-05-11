// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_RopeAim.generated.h"

/**
 * 로프 발사 전 조준을 담당하는 어빌리티입니다.
 */
UCLASS()
class BOSSRUSH_API UGA_RopeAim : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_RopeAim();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void TickAiming();

private:
	UPROPERTY()
	class ACharacterBase* Character;

	UPROPERTY()
	class URopeActionComponent* RopeComponent;

	UPROPERTY(EditAnywhere, Category = "Rope")
	float MaxDistance = 4000.0f;

	FTimerHandle AimTimerHandle;
};
