// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/Boss/GA_PatternBase.h"
#include "GA_AttackPatternBase.generated.h"



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
	EDistance AttackRange = EDistance::Near;
};
