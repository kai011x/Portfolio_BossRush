// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_UltraSkill_Fighter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"

UGA_UltraSkill_Fighter::UGA_UltraSkill_Fighter()
{
	SkillRowName = TEXT("UltraSkill");
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_UltraSkill_Fighter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (UltraSkillMontage)
	{
		// 1. 몽타주 재생 및 종료 대기 태스크 생성
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this, TEXT("UltraSkill"), UltraSkillMontage);

		// 성공적으로 끝났든, 중간에 끊겼든 기술을 종료시킴
		MontageTask->OnCompleted.AddDynamic(this, &UGA_UltraSkill_Fighter::OnMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_UltraSkill_Fighter::OnMontageEnded);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_UltraSkill_Fighter::OnMontageEnded);
		
		MontageTask->ReadyForActivation();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[GA_UltraSkill_Fighter] UltraSkillMontage is NULL!"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UGA_UltraSkill_Fighter::OnMontageEnded()
{
	// 몽타주가 끝나면 어빌리티도 종료
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
