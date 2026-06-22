// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/GA_Hitted.h"
#include "Characters/CharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/Tags/GameplayTags.h"
#include "GameFramework/Actor.h"
#include "Characters/Bosses/BossBase.h"
#include "AIController.h"
#include "GAS/AttributeSets/BossAttributeSet.h"

UGA_Hitted::UGA_Hitted()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;


}

void UGA_Hitted::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ACharacterBase* TargetChar = Cast<ACharacterBase>(ActorInfo->AvatarActor.Get());
	if (!TargetChar)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 보스인 경우 진행 중인 AI 이동과 패턴 상태를 정지/초기화합니다.
	if (ABossBase* Boss = Cast<ABossBase>(TargetChar))
	{
		if (AAIController* AIC = Cast<AAIController>(Boss->GetController()))
		{
			AIC->StopMovement();
		}
		Boss->ResetPatternExecution();
	}

	// 1. 캐릭터에 저장된 최신 피격 정보 획득
	EHitType HitType = TargetChar->LastHitData.HitType;
	int32 HitIdx = TargetChar->LastHitData.HitIdx;
	float LaunchDistance = TargetChar->LastHitData.LaunchDistance;
	float LaunchHeight = TargetChar->LastHitData.LaunchHeight;
	AActor* HitInstigator = TargetChar->LastHitInstigator;

	// 1.1 보스인 경우 그로기 게이지 누적 (Light: 5, Heavy: 10)
	if (UBossAttributeSet* BossAS = Cast<UBossAttributeSet>(TargetChar->GetAttributeSet()))
	{
		float GroggyAmount = 0.0f;
		if (HitType == EHitType::Light)
		{
			GroggyAmount = 5.0f;
		}
		else if (HitType == EHitType::Heavy)
		{
			GroggyAmount = 10.0f;
		}

		if (GroggyAmount > 0.0f)
		{
			float CurrentGroggy = BossAS->GetGroggy();
			float MaxGroggy = BossAS->GetMaxGroggy();
			BossAS->SetGroggy(FMath::Clamp(CurrentGroggy + GroggyAmount, 0.0f, MaxGroggy));
		}
	}

	// 2. 공격자를 바라보는 시선 처리 로직
	if (HitInstigator)
	{
		FVector Direction = HitInstigator->GetActorLocation() - TargetChar->GetActorLocation();
		Direction.Z = 0.0f;
		if (!Direction.IsNearlyZero())
		{
			FRotator NewRot = FRotationMatrix::MakeFromX(Direction).Rotator();
			TargetChar->SetActorRotation(NewRot);
		}
	}

	UAnimMontage* TargetMontage = nullptr;
	float PlayRate = 1.0f;

	if (TargetChar->DT_Hitted)
	{
		static const FString ContextString(TEXT("HitReactLookup"));
		TArray<FHittedData*> AllRows;
		TargetChar->DT_Hitted->GetAllRows<FHittedData>(ContextString, AllRows);

		for (FHittedData* Row : AllRows)
		{
			if (Row && Row->HitType == HitType && Row->idx == HitIdx)
			{
				TargetMontage = Row->Montage;
				PlayRate = Row->PlayRate;
				if (LaunchDistance <= 0.0f) LaunchDistance = Row->LaunchDistance;
				if (LaunchHeight <= 0.0f) LaunchHeight = Row->LaunchHeight;
				break;
			}
		}
	}

	// 물리 효과 적용
	if (LaunchDistance > 0.0f || LaunchHeight > 0.0f)
	{
		FVector LaunchDir = -1 * TargetChar->GetActorForwardVector();
		LaunchDir.Z = 0.0f;
		LaunchDir.Normalize();

		FVector LaunchVelocity = LaunchDir * LaunchDistance;
		LaunchVelocity.Z = LaunchHeight;

		TargetChar->LaunchCharacter(LaunchVelocity, true, true);
	}

	// 몽타주 재생을 위한 AbilityTask 생성 및 실행
	if (TargetMontage)
	{
		UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("HitReactMontageTask"),
			TargetMontage,
			PlayRate
		);

		if (PlayMontageTask)
		{
			PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Hitted::OnMontageFinished);
			PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Hitted::OnMontageFinished);
			PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Hitted::OnMontageFinished);
			PlayMontageTask->ReadyForActivation();
			return;
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Hitted::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Hitted::OnMontageFinished()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
