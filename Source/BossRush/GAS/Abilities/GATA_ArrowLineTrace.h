// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_SingleLineTrace.h"
#include "GATA_ArrowLineTrace.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API AGATA_ArrowLineTrace : public AGameplayAbilityTargetActor_SingleLineTrace
{
	GENERATED_BODY()
	
public:
	AGATA_ArrowLineTrace();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Targeting")
	bool bUsePlayerCameraDirection = false;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
};
