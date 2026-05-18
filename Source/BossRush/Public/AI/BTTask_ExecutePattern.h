// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecutePattern.generated.h"

/**
 * BossBase의 ExecuteNextPattern을 호출하는 BT Task
 */
UCLASS()
class BOSSRUSH_API UBTTask_ExecutePattern : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ExecutePattern();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
