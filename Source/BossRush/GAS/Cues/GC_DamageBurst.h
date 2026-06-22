// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GC_DamageBurst.generated.h"

/**
 * 데미지 피격 시 데미지 숫자를 로컬 클라이언트에 표시하기 위한 게임플레이 큐
 */
UCLASS()
class BOSSRUSH_API UGC_DamageBurst : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UGC_DamageBurst();

	virtual void HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;
};
