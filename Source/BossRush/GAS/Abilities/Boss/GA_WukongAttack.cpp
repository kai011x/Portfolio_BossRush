// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Boss/GA_WukongAttack.h"
#include "Characters/Bosses/BossBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_WukongAttack::UGA_WukongAttack()
{
	PatternType = EPatternType::Attack;
}

void UGA_WukongAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 1. 아바타 캐릭터를 ABossBase로 캐스팅
	ABossBase* Boss = Cast<ABossBase>(GetAvatarActorFromActorInfo());
	if (!Boss)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UDataTable* MontageTable = Boss->BossAttackMontageDataTable;

	EDistance TargetRange = Boss->GetCurrentDistanceKey();
	int32 Montageidx = FMath::RandRange(0, Boss->DistanceMontageCounts[TargetRange] - 1);
	
	
	UAnimMontage* SelectedMontage = nullptr;
	float PlayRate = 1.0f;

	if (MontageTable)
	{
		TArray<FBossMontageData*> OutRowList;
		MontageTable->GetAllRows<FBossMontageData>(TEXT(""), OutRowList);


		FBossMontageData* MatchingData = nullptr;

		for (FBossMontageData* RowData : OutRowList)
		{
			if (RowData && RowData->AttackRange == TargetRange && RowData->idx == Montageidx)
			{
				MatchingData = RowData;
			}
		}

		if (MatchingData)
		{
			SelectedMontage = MatchingData->Montage;
			PlayRate = MatchingData->PlayRate;
		}
	}

	// 5. 몽타주가 존재하면 AbilityTask를 생성하여 재생 후 감시
	if (SelectedMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("WukongAttackTask"),
			SelectedMontage,
			PlayRate
		);

		if (PlayMontageTask)
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_WukongAttack::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_WukongAttack::OnMontageFinished);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_WukongAttack::OnMontageFinished);

			Boss->RotateToTarget();

			PlayMontageTask->ReadyForActivation();
			return;
		}
	}

	// 몽타주가 없거나 타스크 생성 실패 시 즉시 어빌리티 종료
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UGA_WukongAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_WukongAttack::OnMontageFinished()
{
	ABossBase* Boss = Cast<ABossBase>(GetAvatarActorFromActorInfo());
	Boss->ResetPatternExecution(); // 패턴 실행 상태 해제

	// 몽타주 재생이 완료되면 어빌리티를 완전히 종료 처리
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
