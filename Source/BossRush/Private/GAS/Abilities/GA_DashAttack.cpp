// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_DashAttack.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGA_DashAttack::UGA_DashAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_DashAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
	DashAttackTag = FGameplayTag::RequestGameplayTag(FName("Event.Attack.Dash"));

	static const FString ContextString(TEXT("Montage Lookup Context"));

	FMontageData* FoundRow = Character->DT_Montages->FindRow<FMontageData>(DashAttackTag.GetTagName(), ContextString);


	if (FoundRow)
	{


		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			FoundRow->Montage,
			FoundRow->PlayRate
		);


		MontageTask->OnBlendOut.AddDynamic(this, &UGA_DashAttack::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_DashAttack::OnMontageCompleted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_DashAttack::OnMontageCompleted);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_DashAttack::OnMontageCompleted);
		MontageTask->ReadyForActivation();
	}
}


void UGA_DashAttack::OnMontageCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("Dash Attack Montage Completed"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}