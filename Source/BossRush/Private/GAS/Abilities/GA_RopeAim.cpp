// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/GA_RopeAim.h"
#include "Characters/Archer.h"
#include "Components/RopeActionComponent.h"
#include "GAS/Tags/GameplayTags.h"
#include "TimerManager.h"

UGA_RopeAim::UGA_RopeAim()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	AbilityTags.AddTag(FGameplayTags::Get().RopeAimStateTag);
	ActivationOwnedTags.AddTag(FGameplayTags::Get().RopeAimStateTag);

	
	ActivationBlockedTags.AddTag(FGameplayTags::Get().AimingStateTag);
}

void UGA_RopeAim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	RopeComponent = Character->FindComponentByClass<URopeActionComponent>();
	if (RopeComponent)
	{
		RopeComponent->SetAimingTickEnabled(true);
	}

	// Archer 위젯 활성화
	if (AArcher* Archer = Cast<AArcher>(Character))
	{
		Archer->SetRopeAimWidgetVisible(true);
	}
}

void UGA_RopeAim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (Character)
	{
		if (RopeComponent)
		{
			RopeComponent->SetAimingTickEnabled(false);
		}

		if (AArcher* Archer = Cast<AArcher>(Character))
		{
			Archer->SetRopeAimWidgetVisible(false);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
