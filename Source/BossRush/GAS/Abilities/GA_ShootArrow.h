// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_ShootArrow.generated.h"

/**
 * 아처의 화살 발사 처리를 수행하는 게임플레이 어빌리티
 */
UCLASS()
class BOSSRUSH_API UGA_ShootArrow : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_ShootArrow();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& DataHandle);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
	TSubclassOf<class AGATA_ArrowLineTrace> TargetActorClass;
};
