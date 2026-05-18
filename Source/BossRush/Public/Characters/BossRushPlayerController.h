// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BossRushPlayerController.generated.h"

/**
 * 기믹 입력 제어 기능이 포함된 플레이어 컨트롤러
 */
UCLASS()
class BOSSRUSH_API ABossRushPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/** 기믹 모드 활성화/비활성화 (입력 컨텍스트 전환) */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetGimmickInputMode(bool bEnabled);

protected:
	virtual void BeginPlay() override;

protected:
	/** 일반 전투용 입력 컨텍스트 */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	/** 기믹 전용 입력 컨텍스트 (키입력 기믹 등) */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* GimmickMappingContext;
};
