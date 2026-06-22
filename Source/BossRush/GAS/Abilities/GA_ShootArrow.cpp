// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/GA_ShootArrow.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GAS/Abilities/GATA_ArrowLineTrace.h"
#include "Characters/Archer.h"
#include "Characters/Weapons/CArrow.h"
#include "GAS/Tags/GameplayTags.h"

UGA_ShootArrow::UGA_ShootArrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_ShootArrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AArcher* Archer = Cast<AArcher>(ActorInfo->AvatarActor.Get());
	if (!Archer)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	// 1. 타겟 액터 C++ 스폰 및 설정
	TSubclassOf<AGATA_ArrowLineTrace> ClassToSpawn = TargetActorClass ? TargetActorClass : TSubclassOf<AGATA_ArrowLineTrace>(AGATA_ArrowLineTrace::StaticClass());
	AGATA_ArrowLineTrace* TargetActor = GetWorld()->SpawnActor<AGATA_ArrowLineTrace>(ClassToSpawn);
	
	if (TargetActor)
	{
		// 조준 태그 보유 여부 체크
		bool bIsAiming = Archer->GetAbilitySystemComponent() && Archer->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag);

		TargetActor->bDebug = true;
		TargetActor->MaxRange = 10000.0f;
		TargetActor->StartLocation.LocationType = EGameplayAbilityTargetingLocationType::Type::ActorTransform;
		TargetActor->StartLocation.SourceActor = Archer;
		
		// 조준 중일 때만 카메라 방향으로 트레이스, 아닐 때는 아처 액터 정면 방향으로 트레이스
		TargetActor->bUsePlayerCameraDirection = bIsAiming;
	}

	// 2. WaitTargetData Task 생성 및 실행
	UAbilityTask_WaitTargetData* WaitTargetDataTask = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		FName("WaitTargetDataTask"),
		EGameplayTargetingConfirmation::Instant,
		TargetActor
	);

	if (WaitTargetDataTask)
	{
		WaitTargetDataTask->ValidData.AddDynamic(this, &UGA_ShootArrow::OnTargetDataReceived);
		WaitTargetDataTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UGA_ShootArrow::OnTargetDataReceived(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	AArcher* Archer = Cast<AArcher>(CurrentActorInfo->AvatarActor.Get());
	if (Archer && Archer->HasAuthority())
	{
		// 조준 태그 보유 여부 체크
		bool bIsAiming = Archer->GetAbilitySystemComponent() && Archer->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTags::Get().AimingStateTag);
		FVector LaunchDirection = Archer->GetActorForwardVector();

		if (bIsAiming)
		{
			// 조준 상태일 때는 카메라 레이캐스트로 획득한 타겟 포인트 정보를 사용하여 발사 방향을 꺾음
			FVector TargetPoint = FVector::ZeroVector;
			bool bHasHit = false;
			
			if (DataHandle.IsValid(0))
			{
				const FHitResult* HitResult = DataHandle.Get(0)->GetHitResult();
				if (HitResult && HitResult->bBlockingHit)
				{
					TargetPoint = HitResult->ImpactPoint;
					bHasHit = true;
				}
			}
			
			if (bHasHit && !TargetPoint.IsNearlyZero() && Archer->CurrentArrow)
			{
				LaunchDirection = (TargetPoint - Archer->CurrentArrow->GetActorLocation()).GetSafeNormal();
			}
			else
			{
				// 카메라에 부딪힌 게 없을 경우 카메라가 바라보는 방향으로 발사
				APlayerController* PC = CurrentActorInfo->PlayerController.Get();
				if (PC)
				{
					FVector ViewStart;
					FRotator ViewRot;
					PC->GetPlayerViewPoint(ViewStart, ViewRot);
					LaunchDirection = ViewRot.Vector();
				}
				else
				{
					LaunchDirection = Archer->GetActorForwardVector();
				}
			}
		}
		else
		{
			// 조준 상태가 아닐 때는 항상 캐릭터의 정면 전방 방향으로 발사
			LaunchDirection = Archer->GetActorForwardVector();
		}

		// 아처의 화살 발사 로직 호출
		if (Archer->CurrentArrow)
		{
			Archer->CurrentArrow->SetActorHiddenInGame(false);
			Archer->CurrentArrow->Launch(LaunchDirection);
			Archer->CurrentArrow = nullptr;
			Archer->ClearAimTarget();
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
