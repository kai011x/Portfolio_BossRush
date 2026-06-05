// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_PatternBase.h"
#include "GA_AttackPatternBase.generated.h"

/**
 * 보스의 공격 사거리를 정의하는 열거형
 */
UENUM(BlueprintType)
enum class EAttackRange : uint8
{
	Near    UMETA(DisplayName = "Near"),
	Middle  UMETA(DisplayName = "Middle"),
	Far     UMETA(DisplayName = "Far"),
	Max
};

/**
 * 보스의 공격 패턴 어빌리티의 기반 클래스
 */
UCLASS()
class BOSSRUSH_API UGA_AttackPatternBase : public UGA_PatternBase
{
	GENERATED_BODY()

public:
	UGA_AttackPatternBase();

protected:
	/** 공격 사거리 종류 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern|Attack")
	EAttackRange AttackRange = EAttackRange::Near;
};
