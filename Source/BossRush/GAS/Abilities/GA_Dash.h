// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataTable.h"
#include "ActionDatas.h"
#include "GA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_Dash : public UGameplayAbility
{
	GENERATED_BODY()

	UGA_Dash();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 태그로 데이터 테이블의 행을 찾는 함수
	FMontageData* FindMontageDataByTag(UDataTable* Table, FGameplayTag SearchTag);

	// 입력 방향을 태그로 변환하는 함수
	FGameplayTag DetermineDashTag(FVector LocalDirection);

public:
	// 기존 함수 (월드 기준 입력 벡터 반환)
	UFUNCTION(BlueprintCallable, Category = "Ability|Dash")
	FVector GetDashDirection(AActor* SourceActor);

	// 몽타주가 끝나면 호출될 콜백 함수 (반드시 UFUNCTION 필요)
	UFUNCTION()
	void OnMontageEnded();

protected:
	// 2. 에디터에서 할당할 데이터 테이블
	UPROPERTY(BlueprintReadOnly, Category = "Dash Settings")
	UDataTable* DashMontageTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash Settings")
	TSubclassOf<class UGameplayEffect> DashRegenEffectClass;
};
