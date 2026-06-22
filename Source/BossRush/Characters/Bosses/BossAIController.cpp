// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bosses/BossAIController.h"
#include "Components/StateTreeComponent.h"
#include "Characters/Bosses/BossBase.h"

ABossAIController::ABossAIController()
{
	StateTreeComponent = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTreeComponent"));
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (StateTreeComponent)
	{
		StateTreeComponent->StartLogic();
	}
}

void ABossAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	if (ABossBase* Boss = Cast<ABossBase>(GetPawn()))
	{
		Boss->OnChaseCompleted();
	}
}
