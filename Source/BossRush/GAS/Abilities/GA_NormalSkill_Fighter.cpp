// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_NormalSkill_Fighter.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"
#include "GameFramework/Character.h"

UGA_NormalSkill_Fighter::UGA_NormalSkill_Fighter()
{
	SkillRowName = TEXT("NormalSkill");
}

void UGA_NormalSkill_Fighter::ExecuteSkillLogic()
{
	// 부모의 공통 로직(몽타주 재생) 실행
	Super::ExecuteSkillLogic();

	// Fighter만의 추가 로직: 착지 감지 작업 시작
	UAbilityTask_WaitMovementModeChange* WaitLandTask = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, EMovementMode::MOVE_Walking);
	if (WaitLandTask)
	{
		WaitLandTask->OnChange.AddDynamic(this, &UGA_NormalSkill_Fighter::OnLanded);
		WaitLandTask->ReadyForActivation();
	}
}

void UGA_NormalSkill_Fighter::OnLanded(EMovementMode NewMode)
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			// 현재 재생 중인 스킬 몽타주의 'Land' 섹션으로 점프
			if (SkillMontage && AnimInstance->Montage_IsPlaying(SkillMontage))
			{
				AnimInstance->Montage_JumpToSection(FName("Land"), SkillMontage);
			}
		}
	}
}
