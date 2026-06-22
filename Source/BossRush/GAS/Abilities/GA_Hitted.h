// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ActionDatas.h"
#include "GA_Hitted.generated.h"

/**
 * 피격 이벤트 데이터를 담을 상세 UObject 클래스
 */
UCLASS(BlueprintType)
class BOSSRUSH_API UHitReactDetails : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Hit")
	EHitType HitType = EHitType::None;

	UPROPERTY(BlueprintReadWrite, Category = "Hit")
	int32 HitIdx = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Hit")
	float LaunchDistance = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Hit")
	float LaunchHeight = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Hit")
	TObjectPtr<AActor> HitInstigator = nullptr;
};

/**
 * 보스와 캐릭터 공용 피격 리액션 게임플레이 어빌리티
 */
UCLASS()
class BOSSRUSH_API UGA_Hitted : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Hitted();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void OnMontageFinished();
};
