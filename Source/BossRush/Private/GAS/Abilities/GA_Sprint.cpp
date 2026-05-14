// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Sprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h" 
#include "GAS/Tags/GameplayTags.h"



UGA_Sprint::UGA_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// CancelAbilities 대응을 위해 AbilityTags에 태그 설정
	SetAssetTags(FGameplayTagContainer(FGameplayTags::Get().SprintStateTag));

	ActivationOwnedTags.AddTag(FGameplayTags::Get().SprintStateTag);

}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Character = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());

	if (Character && Character->GetCharacterMovement() && Character->GetAttributeSet())
	{
		// 디버그용 로그 (활성화 확인용으로 유지)
		UE_LOG(LogTemp, Warning, TEXT("GA_Sprint Activated!"));

		// 1. 속도 변경 (Sprint)
		SprintSpeed = Character->GetAttributeSet()->GetSprintSpeed();
		Character->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("[GA_Sprint] EndAbility Called. bWasCancelled: %s"), bWasCancelled ? TEXT("True") : TEXT("False"));

	if (Character && Character->GetCharacterMovement() && Character->GetAttributeSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("GA_Sprint Ended!"));

		// 속도 복구 (Run)
		Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetAttributeSet()->GetRunSpeed();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


