// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_PatternBase.generated.h"

/**
 * 보스 패턴의 종류를 정의하는 열거형
 */
UENUM(BlueprintType)
enum class EPatternType : uint8
{
	Attack      UMETA(DisplayName = "Attack"),      // 일반 근접 공격
	Dodge		UMETA(DisplayName = "Dodge"),		// 회피 대기 상태
	Defense      UMETA(DisplayName = "Defense"),    // 공격
	Max
};



/**
 * 모든 보스 패턴(Ability)의 기반이 되는 클래스
 */
UCLASS()
class BOSSRUSH_API UGA_PatternBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PatternBase();

protected:
	/** 이 패턴의 종류 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	EPatternType PatternType = EPatternType::Max;

	/** 패턴 실행 시 재생할 몽타주 (선택 사항) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern|Animation")
	TObjectPtr<UAnimMontage> PatternMontage;
};
