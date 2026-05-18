// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToTarget.h"
#include "Characters/Bosses/BossBase.h"
#include "AIController.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
	NodeName = TEXT("Rotate To Target");
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ABossBase* Boss = Cast<ABossBase>(AIController->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;

	Boss->RotateToTarget();

	return EBTNodeResult::Succeeded;
}
