// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bosses/BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ABossAIController::ABossAIController()
{
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UBlackboardComponent* BBComp = Blackboard;
	if (UseBlackboard(BBAsset, BBComp))
	{
		Blackboard = BBComp;
		RunBehaviorTree(BTAsset);
	}
}
