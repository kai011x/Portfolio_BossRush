// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToTarget.generated.h"

/**
 * BossBase의 RotateToTarget을 호출하는 BT Task
 */
UCLASS()
class BOSSRUSH_API UBTTask_RotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
