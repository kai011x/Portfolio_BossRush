// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossAIController.generated.h"

class UStateTreeComponent;

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
	/** 사용할 State Tree 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateTreeComponent> StateTreeComponent;
};
