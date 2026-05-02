// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "MageAttributeSet.generated.h"

/**
 * MageAttributeSet: BasicAttributeSet을 상속받는 Mage 전용 속성 세트
 */
UCLASS()
class BOSSRUSH_API UMageAttributeSet : public UBasicAttributeSet
{
	GENERATED_BODY()

public:
	UMageAttributeSet();
};
