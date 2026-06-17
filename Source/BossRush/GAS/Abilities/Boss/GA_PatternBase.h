// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/Bosses/BossBase.h"
#include "GA_PatternBase.generated.h"



/**
 * 모든 보스 패턴(Ability)의 기반이 되는 클래스
 */
UCLASS()
class BOSSRUSH_API UGA_PatternBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PatternBase();

	/** 패턴의 대분류 타입을 반환 */
	UFUNCTION(BlueprintCallable, Category = "Pattern")
	EPatternType GetPatternType() const { return PatternType; }

protected:
	/** 이 패턴의 종류 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	EPatternType PatternType = EPatternType::Max;

	/** 패턴 실행 시 재생할 몽타주 (선택 사항) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern|Animation")
	TObjectPtr<UAnimMontage> PatternMontage;
};
