// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_NormalSkill_Fighter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "ActionDatas.h"

UGA_NormalSkill_Fighter::UGA_NormalSkill_Fighter()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_NormalSkill_Fighter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) return;

	ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());

	FActionData* FoundRow = Character->DT_Skills->FindRow<FActionData>(FName("NormalSkill"), TEXT("Find NormalSkill in GA"));
	NormalSkillMontage = FoundRow->Montage;

	if (FoundRow)
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, NormalSkillMontage);

		MontageTask->OnCompleted.AddDynamic(this, &UGA_NormalSkill_Fighter::OnMontageEnded);
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_NormalSkill_Fighter::OnMontageEnded); // 종료 시 처리
		MontageTask->ReadyForActivation();
	
	
	
		UAbilityTask_WaitMovementModeChange* WaitLandTask = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, EMovementMode::MOVE_Walking);

		// 2. 상태가 변하면 실행될 함수(OnLanded) 연결
		WaitLandTask->OnChange.AddDynamic(this, &UGA_NormalSkill_Fighter::OnLanded);

		// 3. 태스크 활성화
		WaitLandTask->ReadyForActivation();
	
	}

}

void UGA_NormalSkill_Fighter::OnMontageEnded()
{
	// 여기서 진짜 EndAbility를 호출 (파라미터 자동 채움)
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_NormalSkill_Fighter::OnLanded(EMovementMode NewMode)
{
	UE_LOG(LogTemp, Warning, TEXT("Landed during Normal Skill"));

	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			// 2. 현재 몽타주가 있으면 'Land' 섹션으로 점프합니다.
			// (CurrentMontage는 이 어빌리티가 재생시킨 몽타주여야 합니다.)
			if (AnimInstance->Montage_IsPlaying(NormalSkillMontage))
			{
				AnimInstance->Montage_JumpToSection(FName("Land"), NormalSkillMontage);
			}
		}
	}
}