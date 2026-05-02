// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_ComboAttack.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "Animation/AnimMontage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

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
	CurrentMontageTask = nullptr; // �ʱ�ȭ

	ACharacterBase* Character = Cast<ACharacterBase>(GetAvatarActorFromActorInfo());
	if (Character)
	{

		if (InputTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Event.Attack.NormalInput"))))
		{
			ComboDataTable = Character->DT_NormalAttackCombo;
		}
		else if (InputTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Event.Attack.StrongInput"))))
		{
			ComboDataTable = Character->DT_StrongAttackCombo;
		}


	}


	InputWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		InputTag, // ��ٸ� �±�
		nullptr,
		false, // �߿�: �� �� �ް� ������ ���� (��� ���)
		true   // �±� ��Ȯ�� ��ġ
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
		// �� �̻� �޺��� ������ ����
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
	
	// ��������Ʈ ����
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_ComboAttack::OnMontageCompleted);
	MontageTask->ReadyForActivation();
	
	
	UAbilityTask_WaitGameplayEvent* CheckWaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		CheckTag, // ��ٸ� �±�: Event.Attack.Check
		nullptr,
		true, // ��ȣ�� �� �� ������ �� �½�ũ�� �� ���� ���� ���� (���� Step���� ���� ����)
		true
	);


	CheckWaitTask->EventReceived.AddDynamic(this, &UGA_ComboAttack::OnComboCheckEvent);
	CheckWaitTask->ReadyForActivation();
}

void UGA_ComboAttack::OnInputReceived(FGameplayEventData Payload)
{
	// ������ Ŭ���� -> ���� �޺� ����
	bNextInputPressed = true;
}


void UGA_ComboAttack::OnComboCheckEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Check Event Received. Next Input Pressed: %s"), bNextInputPressed ? TEXT("True") : TEXT("False"));

	if (bNextInputPressed)
	{
		// �Է��� �־����� ���� �ܰ�� ����
		CurrentComboIndex++;

		UE_LOG(LogTemp, Warning, TEXT("Proceeding to Next Combo Index: %d"), CurrentComboIndex);

		PlayComboAction(); // ��� ȣ��ó�� ��������, ���ο� �½�ũ���� ������
	}
	else
	{
		// �Է��� �������� �ƹ��͵� �� �� (��Ÿ�ְ� BlendOut �� ������ ����ǰ� �����)
	}
}

void UGA_ComboAttack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


