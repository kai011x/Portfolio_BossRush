// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "TankerAttributeSet.generated.h"

/**
 * TankerAttributeSet: BasicAttributeSet을 상속받는 Tanker 전용 속성 세트
 */
UCLASS()
class BOSSRUSH_API UTankerAttributeSet : public UBasicAttributeSet
{
	GENERATED_BODY()

public:
	UTankerAttributeSet();
};
