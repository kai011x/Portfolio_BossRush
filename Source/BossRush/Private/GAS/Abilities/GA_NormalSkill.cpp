// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_NormalSkill.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/Tags/GameplayTags.h"

UGA_NormalSkill::UGA_NormalSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationOwnedTags.AddTag(FGameplayTags::Get().NormalSkillEventTag);
}

void UGA_NormalSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	if (Character && Character->DT_Skills)
	{
		FActionData* FoundRow = Character->DT_Skills->FindRow<FActionData>(SkillRowName, TEXT("Find Skill in GA_NormalSkill"));
		if (FoundRow)
		{
			SkillMontage = FoundRow->Montage;
		}
	}

	ExecuteSkillLogic();
}

void UGA_NormalSkill::ExecuteSkillLogic()
{
	if (SkillMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SkillMontage);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_NormalSkill::OnSkillMontageEnded);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_NormalSkill::OnSkillMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_NormalSkill::OnSkillMontageEnded);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_NormalSkill::OnSkillMontageEnded);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGA_NormalSkill::OnSkillMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
