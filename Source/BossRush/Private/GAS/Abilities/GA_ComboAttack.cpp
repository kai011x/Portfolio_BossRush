// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_ComboAttack.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "Animation/AnimMontage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"


UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Attack_Input, "Event.Attack.Input");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Attack_Check, "Event.Attack.Check");

UGA_ComboAttack::UGA_ComboAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboIndex = 0;
	bNextInputPressed = false;
}

void UGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CurrentComboIndex = 0;
	CurrentMontageTask = nullptr; // 초기화

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		// 캐릭터가 가진 데이터 테이블 참조
		ComboDataTable = Character->DT_NormalAttackCombo;
	}


	InputWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_Event_Attack_Input, // 기다릴 태그: Event.Attack.Input
		nullptr,
		false, // 중요: 한 번 받고 끝내지 않음 (계속 대기)
		true   // 태그 정확히 일치
	);

	UE_LOG(LogTemp, Warning, TEXT("Combo Ability Activated"));

	if (InputWaitTask)
	{
		InputWaitTask->EventReceived.AddDynamic(this, &UGA_ComboAttack::OnInputReceived);
		InputWaitTask->ReadyForActivation();
	}

	PlayComboAction();
}

void UGA_ComboAttack::PlayComboAction()
{
	if (!ComboDataTable)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	TArray<FActionData*> ComboRows;
	ComboDataTable->GetAllRows<FActionData>(TEXT("GetAllRows"), ComboRows);

	if (!ComboRows.IsValidIndex(CurrentComboIndex))
	{
		// 더 이상 콤보가 없으면 종료
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Playing Combo Index: %d"), CurrentComboIndex);

	FActionData* CurrentAction = ComboRows[CurrentComboIndex];
	UAnimMontage* MontageToPlay = CurrentAction->Montage;

	if (CurrentMontageTask)
	{
		CurrentMontageTask->OnBlendOut.RemoveDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
		CurrentMontageTask->OnInterrupted.RemoveDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
		CurrentMontageTask->OnCancelled.RemoveDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
		CurrentMontageTask = nullptr;
	}


	bNextInputPressed = false;

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		MontageToPlay,
		CurrentAction->PlayRate
	);
	
	CurrentMontageTask = MontageTask;
	
	// 델리게이트 연결
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
	MontageTask->ReadyForActivation();
	
	
	UAbilityTask_WaitGameplayEvent* CheckWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		TAG_Event_Attack_Check, // 기다릴 태그: Event.Attack.Check
		nullptr,
		true, // 신호를 한 번 받으면 이 태스크는 할 일을 다한 것임 (다음 Step에서 새로 만듦)
		true
	);

	UE_LOG(LogTemp, Warning, TEXT("Waiting for Combo Check Event"));

	CheckWaitTask->EventReceived.AddDynamic(this, &UGA_ComboAttack::OnComboCheckEvent);
	CheckWaitTask->ReadyForActivation();
}

void UGA_ComboAttack::OnInputReceived(FGameplayEventData Payload)
{
	// 유저가 클릭함 -> 다음 콤보 예약
	bNextInputPressed = true;

	// 로그 등으로 확인 가능
	
	UE_LOG(LogTemp, Warning, TEXT("Input Buffered!"));
}


void UGA_ComboAttack::OnComboCheckEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Check Event Received. Next Input Pressed: %s"), bNextInputPressed ? TEXT("True") : TEXT("False"));

	if (bNextInputPressed)
	{
		// 입력이 있었으면 다음 단계로 진행
		CurrentComboIndex++;

		UE_LOG(LogTemp, Warning, TEXT("Proceeding to Next Combo Index: %d"), CurrentComboIndex);

		PlayComboAction(); // 재귀 호출처럼 보이지만, 새로운 태스크들을 생성함
	}
	else
	{
		// 입력이 없었으면 아무것도 안 함 (몽타주가 BlendOut 될 때까지 재생되고 종료됨)
	}
}

void UGA_ComboAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


