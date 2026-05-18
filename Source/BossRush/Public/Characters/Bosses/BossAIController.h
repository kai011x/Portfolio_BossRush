// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

/**
 * 보스 전용 AI 컨트롤러
 */
UCLASS()
class BOSSRUSH_API ABossAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABossAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	/** 실행할 비헤이비어 트리 */
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BTAsset;

	/** 사용할 블랙보드 데이터 */
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBlackboardData* BBAsset;
};
