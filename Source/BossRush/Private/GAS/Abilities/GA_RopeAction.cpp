// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/GA_RopeAction.h"
#include "Characters/Archer.h"
#include "Characters/Weapons/ACRope.h"
#include "Components/RopeActionComponent.h"
#include "GAS/Tags/GameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

UGA_RopeAction::UGA_RopeAction()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_RopeAction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	RopeComponent = Character->FindComponentByClass<URopeActionComponent>();
	if (!RopeComponent || !RopeComponent->Rope)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	TargetLocation = RopeComponent->GetTargetLocation();

	// 발사 애니메이션 재생
	if (FireMontage)
	{
		Character->PlayAnimMontage(FireMontage);
	}

	RopeComponent->Rope->OnRopeAttached.AddDynamic(this, &UGA_RopeAction::OnRopeAttached);
	RopeComponent->Rope->Fire(TargetLocation);
	
	FGameplayTagContainer ActionTagContainer;
	ActionTagContainer.AddTag(FGameplayTags::Get().RopeActionStateTag);
	Character->GetAbilitySystemComponent()->AddLooseGameplayTags(ActionTagContainer);
}

void UGA_RopeAction::OnRopeAttached()
{
	if (!Character || !RopeComponent || !RopeComponent->Rope) return;

	RopeComponent->Rope->OnRopeAttached.RemoveDynamic(this, &UGA_RopeAction::OnRopeAttached);

	// 이동 애니메이션 재생
	if (MoveMontage)
	{
		Character->PlayAnimMontage(MoveMontage);
	}

	bIsMoving = true;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Character->GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &UGA_RopeAction::TickMovement, 0.016f, true);
}

void UGA_RopeAction::TickMovement()
{
	if (!Character || !bIsMoving) return;

	FVector CurrentLocation = Character->GetActorLocation();
	float DistanceToTarget = FVector::Dist(CurrentLocation, TargetLocation);

	if (DistanceToTarget <= StopDistance)
	{
		FinishMovement();
		return;
	}

	float DeltaTime = 0.016f;
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed / 100.0f);
	
	FHitResult SweepHit;
	Character->SetActorLocation(NewLocation, true, &SweepHit);
	
	if (SweepHit.bBlockingHit)
	{
		FinishMovement();
	}
}

void UGA_RopeAction::FinishMovement()
{
	if (!Character) return;

	Character->GetWorldTimerManager().ClearTimer(MoveTimerHandle);
	bIsMoving = false;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_RopeAction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (Character)
	{
		Character->GetWorldTimerManager().ClearTimer(MoveTimerHandle);
		
		if (FireMontage) Character->StopAnimMontage(FireMontage);
		if (MoveMontage) Character->StopAnimMontage(MoveMontage);

		FGameplayTagContainer ActionTagContainer;
		ActionTagContainer.AddTag(FGameplayTags::Get().RopeActionStateTag);
		Character->GetAbilitySystemComponent()->RemoveLooseGameplayTags(ActionTagContainer);
		
		if (RopeComponent && RopeComponent->Rope)
		{
			RopeComponent->Rope->OnRopeAttached.RemoveDynamic(this, &UGA_RopeAction::OnRopeAttached);
			RopeComponent->Rope->Retract();
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
