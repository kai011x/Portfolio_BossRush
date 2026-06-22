// GA_BossDash.cpp

#include "GAS/Abilities/Boss/GA_BossDash.h"
#include "GAS/Tags/GameplayTags.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Characters/CharacterBase.h"
#include "Characters/Bosses/BossBase.h"

UGA_BossDash::UGA_BossDash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UGA_BossDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABossBase* Boss = Cast<ABossBase>(ActorInfo->AvatarActor.Get());
	if (!Boss)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	DashMontageTable = Boss->DT_Montages;

	if (!DashMontageTable)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}


	// 2. 데이터 테이블에서 랜덤으로 행을 선택하여 몽타주 가져오기
	FMontageData* FoundData = nullptr;
	TArray<FName> RowNames = DashMontageTable->GetRowNames();
	FName SelectedRowName = NAME_None;
	if (RowNames.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);
		SelectedRowName = RowNames[RandomIndex];
		FoundData = DashMontageTable->FindRow<FMontageData>(SelectedRowName, TEXT("RandomBossDash"));
	}

	if (FoundData && FoundData->Montage)
	{

		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			FName("BossDashTask"),
			FoundData->Montage,
			FoundData->PlayRate,
			FName("DefaultSlot"),
			false
		);

		Task->OnCompleted.AddDynamic(this, &UGA_BossDash::OnMontageEnded);
		Task->OnBlendOut.AddDynamic(this, &UGA_BossDash::OnMontageEnded);
		Task->OnInterrupted.AddDynamic(this, &UGA_BossDash::OnMontageEnded);
		Task->OnCancelled.AddDynamic(this, &UGA_BossDash::OnMontageEnded);

		Boss->RotateToTarget();

		Task->ReadyForActivation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find random Boss Dash Montage from Table"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}


void UGA_BossDash::OnMontageEnded()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;

	ABossBase* Boss = Cast<ABossBase>(CurrentActorInfo->AvatarActor.Get());
	if (!Boss)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}

	Boss->bIsExecutingPattern = false;

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
