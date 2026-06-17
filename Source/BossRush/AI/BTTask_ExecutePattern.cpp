// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ExecutePattern.h"
#include "Characters/Bosses/BossBase.h"
#include "AIController.h"

UBTTask_ExecutePattern::UBTTask_ExecutePattern()
{
	NodeName = TEXT("Execute Pattern");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ExecutePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ABossBase* Boss = Cast<ABossBase>(AIController->GetPawn());
	if (!Boss) return EBTNodeResult::Failed;


	return EBTNodeResult::InProgress;
}

void UBTTask_ExecutePattern::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) 
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ABossBase* Boss = Cast<ABossBase>(AIController->GetPawn());
	if (Boss && !Boss->IsExecutingPattern()) // BossBase에 IsExecutingPattern Getter 추가 필요
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
