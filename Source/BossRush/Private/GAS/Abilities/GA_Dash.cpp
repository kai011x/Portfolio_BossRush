// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGA_Dash::UGA_Dash()
{
	// 기본 설정
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}


FVector UGA_Dash::GetDashDirection(AActor* SourceActor)
{
	ACharacter* Character = Cast<ACharacter>(SourceActor);
	if (!Character)
	{
		// 캐릭터가 아니면 그냥 액터의 정면 반환
		return SourceActor->GetActorForwardVector();
	}

	FVector InputVector = Character->GetLastMovementInputVector();
	
	if (InputVector.IsNearlyZero())
	{
		return Character->GetActorForwardVector();
	}

	return InputVector.GetSafeNormal();

}
