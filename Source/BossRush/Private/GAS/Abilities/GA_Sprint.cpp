// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Sprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h" 



UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());

	if (Character && Character->GetCharacterMovement())
	{
		// 1. 속도 증가 (Sprint)

		SprintSpeed = Character->GetAttributeSet()->GetSprintSpeed();
		WalkSpeed = Character->GetAttributeSet()->GetRunSpeed();

		Character->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}


