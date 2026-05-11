// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_ArcherAimAttack.h"
#include "Characters/Archer.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/GameplayAbilityTargetActor.h"

UGA_ArcherAimAttack::UGA_ArcherAimAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationBlockedTags.AddTag(FMyGameplayTags::Get().RopeAimStateTag);
}

void UGA_ArcherAimAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AArcher* Archer = Cast<AArcher>(GetAvatarActorFromActorInfo());
	if (!Archer)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AGameplayAbilityTargetActor* TargetActor = Archer->GetOrCreateTargetActor();
	if (!TargetActor)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_WaitTargetData* WaitTargetTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::Instant,
		TargetActor
	);

	WaitTargetTask->ValidData.AddDynamic(this, &UGA_ArcherAimAttack::OnTargetDataReceived);
	WaitTargetTask->Cancelled.AddDynamic(this, &UGA_ArcherAimAttack::OnTargetDataCancelled);
	WaitTargetTask->ReadyForActivation();
}

void UGA_ArcherAimAttack::OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	AArcher* Archer = Cast<AArcher>(GetAvatarActorFromActorInfo());
	if (Archer && DataHandle.Num() > 0)
	{
		const FHitResult* HitResult = DataHandle.Get(0)->GetHitResult();
		if (HitResult)
		{
			Archer->SetAimTargetLocation(HitResult->ImpactPoint);
		}
	}

	// 몽타주 재생은 Archer 클래스에서 처리하거나 여기서 직접 처리 가능
	// 여기서는 Archer 클래스의 AttackMontage를 사용하도록 구현
	if (Archer)
	{
		Archer->PlayAnimMontage(Archer->AttackMontage);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_ArcherAimAttack::OnTargetDataCancelled(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
