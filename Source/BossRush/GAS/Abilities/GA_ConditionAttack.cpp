// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_ConditionAttack.h"
#include "GAS/Tags/GameplayTags.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UGA_ConditionAttack::UGA_ConditionAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// 로프 액션 중에는 공격 불가
	ActivationBlockedTags.AddTag(FGameplayTags::Get().RopeAimStateTag);
	ActivationBlockedTags.AddTag(FGameplayTags::Get().RopeActionStateTag);
	
	CurrentComboIndex = 0;
	bHitSuccessful = false;
}

void UGA_ConditionAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentComboIndex = 0;

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		ComboDataTable = Character->DT_StrongAttackCombo;
	}

	if (!ComboDataTable)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	PlayComboAction();
}

void UGA_ConditionAttack::PlayComboAction()
{
	TArray<FActionData*> ComboRows;
	ComboDataTable->GetAllRows<FActionData>(TEXT("GetAllRows"), ComboRows);

	if (!ComboRows.IsValidIndex(CurrentComboIndex))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	FActionData* CurrentAction = ComboRows[CurrentComboIndex];
	UAnimMontage* MontageToPlay = CurrentAction->Montage;

	UE_LOG(LogTemp, Warning, TEXT("[GA_ConditionAttack] Playing Montage: %s (Index: %d)"), MontageToPlay ? *MontageToPlay->GetName() : TEXT("None"), CurrentComboIndex);

	// 중요: 이전 몽타주 태스크가 종료되면서 EndAbility를 호출하지 못하도록 델리게이트 정리
	if (CurrentMontageTask)
	{
		CurrentMontageTask->OnCompleted.Clear();
		CurrentMontageTask->OnBlendOut.Clear();
		CurrentMontageTask->OnInterrupted.Clear();
		CurrentMontageTask->OnCancelled.Clear();
		CurrentMontageTask->EndTask();
	}

	// 상태 초기화
	bHitSuccessful = false;

	// 기존 태스크들 정리
	if (HitWaitTask) HitWaitTask->EndTask();
	if (CheckWaitTask) CheckWaitTask->EndTask();

	// 1. 적중 성공 이벤트 대기 (단순히 성공 여부만 기록)
	HitWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTags::Get().EventAttackSuccessTag, nullptr, true, true);
	HitWaitTask->EventReceived.AddDynamic(this, &UGA_ConditionAttack::OnHitSuccessReceived);
	HitWaitTask->ReadyForActivation();

	// 2. 연계 체크 지점 대기 (AnimNotify에서 발생)
	CheckWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTags::Get().EventAttackCheckTag, nullptr, true, true);
	CheckWaitTask->EventReceived.AddDynamic(this, &UGA_ConditionAttack::OnCheckEventReceived);
	CheckWaitTask->ReadyForActivation();

	// 3. 몽타주 재생
	CurrentMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, MontageToPlay, CurrentAction->PlayRate);
	CurrentMontageTask->OnCompleted.AddDynamic(this, &UGA_ConditionAttack::OnMontageCompleted);
	CurrentMontageTask->OnBlendOut.AddDynamic(this, &UGA_ConditionAttack::OnMontageCompleted);
	CurrentMontageTask->OnInterrupted.AddDynamic(this, &UGA_ConditionAttack::OnMontageCompleted);
	CurrentMontageTask->OnCancelled.AddDynamic(this, &UGA_ConditionAttack::OnMontageCompleted);
	CurrentMontageTask->ReadyForActivation();
}

void UGA_ConditionAttack::OnHitSuccessReceived(FGameplayEventData Payload)
{
	// 공격이 적중함!
	bHitSuccessful = true;
	UE_LOG(LogTemp, Warning, TEXT("ConditionAttack: Hit Success Recorded. Waiting for Check Event..."));
}

void UGA_ConditionAttack::OnCheckEventReceived(FGameplayEventData Payload)
{
	// 몽타주 내의 Check 지점에 도달했을 때, 적중 성공 상태라면 다음 공격 실행
	if (bHitSuccessful)
	{
		CurrentComboIndex++;
		UE_LOG(LogTemp, Warning, TEXT("ConditionAttack: Check Point Reached & Hit Successful. Moving to Index: %d"), CurrentComboIndex);
		PlayComboAction();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ConditionAttack: Check Point Reached but NO Hit. Will end after montage."));
	}
}

void UGA_ConditionAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
