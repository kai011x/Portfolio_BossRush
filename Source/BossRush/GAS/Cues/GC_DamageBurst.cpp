// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Cues/GC_DamageBurst.h"
#include "Characters/BossRushPlayerController.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Pawn.h"

UGC_DamageBurst::UGC_DamageBurst()
{
	GameplayCueTag = FGameplayTag::RequestGameplayTag(FName("GameplayCue.Damage.Burst"));
}

void UGC_DamageBurst::HandleGameplayCue(AActor* MyTarget, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(MyTarget, EventType, Parameters);

	// 오직 데미지를 입힌 공격자(Local Player)의 화면에만 대미지를 표시합니다.
	APawn* InstigatorPawn = Cast<APawn>(Parameters.Instigator.Get());
	if (InstigatorPawn && InstigatorPawn->IsLocallyControlled())
	{
		float Damage = Parameters.RawMagnitude;
		FVector HitLocation = MyTarget ? MyTarget->GetActorLocation() : FVector::ZeroVector;

		// HitResult의 ImpactPoint를 획득하여 출력 위치로 사용
		if (Parameters.EffectContext.IsValid())
		{
			if (const FHitResult* HitResult = Parameters.EffectContext.GetHitResult())
			{
				if (HitResult->bBlockingHit)
				{
					HitLocation = HitResult->ImpactPoint;
				}
			}
		}

		// 치명타 여부 획득
		bool bIsCritical = Parameters.GameplayCueTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Data.Damage.CrHitical")));

		// 로컬 플레이어 컨트롤러의 델리게이트 브로드캐스트
		if (ABossRushPlayerController* PC = Cast<ABossRushPlayerController>(InstigatorPawn->GetController()))
		{
			PC->OnDamageDealt.Broadcast(MyTarget, Damage, HitLocation, bIsCritical);
		}
	}
}
