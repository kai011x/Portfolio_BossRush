// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataTable.h"
#include "ActionDatas.h"
#include "GA_BossDash.generated.h"

UENUM(BlueprintType)
enum class EBossDashDirectionMode : uint8
{
	Forward      UMETA(DisplayName = "Forward (Boss Forward)"),
	TowardTarget UMETA(DisplayName = "Toward Target (Chase)"),
	AwayFromTarget UMETA(DisplayName = "Away From Target (Evade)")
};

/**
 * 보스 캐릭터 전용 대시 게임플레이 어빌리티
 */
UCLASS()
class BOSSRUSH_API UGA_BossDash : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_BossDash();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


public:

	UFUNCTION()
	void OnMontageEnded();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Dash Settings")
	UDataTable* DashMontageTable;


};
