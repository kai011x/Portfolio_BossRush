// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Bosses/BossAIController.h"
#include "Components/StateTreeComponent.h"

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
