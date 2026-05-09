// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_UltraSkill.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/Tags/GameplayTags.h"

UGA_UltraSkill::UGA_UltraSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ActivationOwnedTags.AddTag(FGameplayTags::Get().UltraSkillEventTag);
}

void UGA_UltraSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	if (Character && Character->DT_Skills)
	{
		FActionData* FoundRow = Character->DT_Skills->FindRow<FActionData>(SkillRowName, TEXT("Find Skill in GA_UltraSkill"));
		if (FoundRow)
		{
			SkillMontage = FoundRow->Montage;
		}
	}

	ExecuteSkillLogic();
}

void UGA_UltraSkill::ExecuteSkillLogic()
{
	if (SkillMontage)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SkillMontage);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_UltraSkill::OnSkillMontageEnded);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_UltraSkill::OnSkillMontageEnded);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_UltraSkill::OnSkillMontageEnded);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_UltraSkill::OnSkillMontageEnded);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGA_UltraSkill::OnSkillMontageEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
