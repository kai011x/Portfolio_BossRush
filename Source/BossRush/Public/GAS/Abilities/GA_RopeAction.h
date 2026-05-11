// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_RopeAction.generated.h"

/**
 * 로프 발사 후 실제 이동을 담당하는 어빌리티입니다.
 */
UCLASS()
class BOSSRUSH_API UGA_RopeAction : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_RopeAction();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void OnRopeAttached();

	void TickMovement();
	void FinishMovement();

private:
	UPROPERTY()
	class ACharacterBase* Character;

	UPROPERTY()
	class URopeActionComponent* RopeComponent;

	UPROPERTY(EditAnywhere, Category = "Rope")
	float MoveSpeed = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Rope")
	float StopDistance = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Rope|Animation")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Rope|Animation")
	UAnimMontage* MoveMontage;

	FVector TargetLocation;
	bool bIsMoving = false;

	FTimerHandle MoveTimerHandle;
};
