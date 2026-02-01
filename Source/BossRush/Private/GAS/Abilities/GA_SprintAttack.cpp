// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_SprintAttack.h"
#include "Characters/CharacterBase.h"
#include "ActionDatas.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UGA_SprintAttack::UGA_SprintAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_SprintAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacterBase* Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
	SprintAttackTag = FGameplayTag::RequestGameplayTag(FName("Event.Attack.Sprint"));
	
	static const FString ContextString(TEXT("Montage Lookup Context"));

	FMontageData* FoundRow = Character->DT_Montages->FindRow<FMontageData>(SprintAttackTag.GetTagName(), ContextString);
	

	if(FoundRow)
	{
		

		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			FoundRow->Montage,
			FoundRow->PlayRate
		);
		

		MontageTask->OnBlendOut.AddDynamic(this, &UGA_SprintAttack::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_SprintAttack::OnMontageCompleted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_SprintAttack::OnMontageCompleted);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_SprintAttack::OnMontageCompleted);
		MontageTask->ReadyForActivation();
	}


}

void UGA_SprintAttack::OnMontageCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Attack Montage Completed"));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}