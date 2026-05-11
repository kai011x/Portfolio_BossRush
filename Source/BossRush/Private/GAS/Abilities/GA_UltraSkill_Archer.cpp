// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_UltraSkill_Archer.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"

UGA_UltraSkill_Archer::UGA_UltraSkill_Archer()
{
	SkillRowName = TEXT("UltSkill");
}

void UGA_UltraSkill_Archer::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	if (Character && Character->DT_Skills)
	{
		FMontageData* FoundRow = Character->DT_Skills->FindRow<FMontageData>(SkillRowName, TEXT("Find Skill in GA_UltraSkill_Archer"));
		if (FoundRow)
		{
			SkillMontage = FoundRow->Montage;
		}
	}

	ExecuteSkillLogic();
}

void UGA_UltraSkill_Archer::ExecuteSkillLogic()
{
	Super::ExecuteSkillLogic();
}
