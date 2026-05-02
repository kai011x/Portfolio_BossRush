#include "GAS/Abilities/GA_UltraSkill_Fighter.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "ActionDatas.h"
#include "Characters/CharacterBase.h"

UGA_UltraSkill_Fighter::UGA_UltraSkill_Fighter()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_UltraSkill_Fighter::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());

	FActionData* FoundRow = Character->DT_Skills->FindRow<FActionData>(FName("UltSkill_Start"), TEXT("Find UltSkill_Start in GA"));
	StartLoopMontage = FoundRow->Montage;

	FoundRow = Character->DT_Skills->FindRow<FActionData>(FName("UltSkill_End"), TEXT("Find UltSkill_End in GA"));
	FinishMontage = FoundRow->Montage;

	UE_LOG(LogTemp, Warning, TEXT("Ultra Skill: Activated"));

	if (!StartLoopMontage || !FinishMontage)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	bInputReceived = false;
	CurrentLoopCount = 0;

	PlayStartStep();
}

void UGA_UltraSkill_Fighter::PlayStartStep()
{
	PlayActionStart();
}

void UGA_UltraSkill_Fighter::OnAbilityEnded()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_UltraSkill_Fighter::StartInputListener()
{
	UAbilityTask_WaitInputPress* InputTask = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
	InputTask->OnPress.AddDynamic(this, &UGA_UltraSkill_Fighter::OnInputPressed);
	InputTask->ReadyForActivation();
}

void UGA_UltraSkill_Fighter::PlayComboStep()
{
}

void UGA_UltraSkill_Fighter::OnInputPressed(float TimeWaited)
{
	UE_LOG(LogTemp, Warning, TEXT("Ultra Skill: Input Pressed"));
	bInputReceived = true;

	StartInputListener();
}

void UGA_UltraSkill_Fighter::PlayActionStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Ultra Skill: Start"));

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, StartLoopMontage, 1.0f, FName("Default")
	);

	Task->OnBlendOut.AddDynamic(this, &UGA_UltraSkill_Fighter::OnStepCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_UltraSkill_Fighter::OnStepCompleted);
	Task->ReadyForActivation();
}

void UGA_UltraSkill_Fighter::PlayLoopStep()
{
	UE_LOG(LogTemp, Warning, TEXT("Ultra Skill: Loop Step %d"), CurrentLoopCount + 1);

	bInputReceived = false;

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, StartLoopMontage, 1.0f, FName("Loop")
	);

	CurrentLoopCount++;

	Task->OnBlendOut.AddDynamic(this, &UGA_UltraSkill_Fighter::OnStepCompleted);
	Task->OnInterrupted.AddDynamic(this, &UGA_UltraSkill_Fighter::OnStepCompleted);
	Task->ReadyForActivation();
}

void UGA_UltraSkill_Fighter::PlayFinishStep()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, FinishMontage, 1.0f, NAME_None
	);

	Task->OnBlendOut.AddDynamic(this, &UGA_UltraSkill_Fighter::OnAbilityEnded);
	Task->ReadyForActivation();
}

void UGA_UltraSkill_Fighter::OnStepCompleted()
{
	if (bInputReceived && CurrentLoopCount < MaxLoopCount)
	{
		PlayLoopStep();
	}
	else
	{
		PlayFinishStep();
	}
}
