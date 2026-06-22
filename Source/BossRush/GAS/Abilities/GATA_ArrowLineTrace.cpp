// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/Abilities/GATA_ArrowLineTrace.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"

AGATA_ArrowLineTrace::AGATA_ArrowLineTrace()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGATA_ArrowLineTrace::Tick(float DeltaSeconds)
{
	AActor::Tick(DeltaSeconds);

	if (bDebug)
	{
		AActor* Source = StartLocation.SourceActor;
		if (Source)
		{
			FHitResult HitResult = PerformTrace(Source);
			FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
			if (TraceStart.IsNearlyZero())
			{
				TraceStart = Source->GetActorLocation();
			}

			FVector Direction = Source->GetActorForwardVector();
			APlayerController* PC = nullptr;
			if (APawn* Pawn = Cast<APawn>(Source))
			{
				PC = Cast<APlayerController>(Pawn->GetController());
			}

			// 조준 중일 경우 시작점을 카메라 시선 원점으로 일치시켜 시각적 틀어짐 보정
			if (bUsePlayerCameraDirection && PC)
			{
				FVector ViewStart;
				FRotator ViewRot;
				PC->GetPlayerViewPoint(ViewStart, ViewRot);
				TraceStart = ViewStart;
				Direction = ViewRot.Vector();
			}

			FVector TraceEnd = TraceStart + (Direction * MaxRange);
			if (HitResult.bBlockingHit)
			{
				TraceEnd = HitResult.ImpactPoint;
			}

			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, -1.0f, 0, 1.5f);
		}
	}
}

FHitResult AGATA_ArrowLineTrace::PerformTrace(AActor* InSourceActor)
{
	if (bUsePlayerCameraDirection)
	{
		// 카메라 시선 방향 트레이스를 C++로 직접 구현 (OwningAbility가 없어도 정상 작동하도록 보장)
		FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
		if (TraceStart.IsNearlyZero() && InSourceActor)
		{
			TraceStart = InSourceActor->GetActorLocation();
		}

		FVector TraceEnd = TraceStart + (InSourceActor ? InSourceActor->GetActorForwardVector() * MaxRange : FVector::ForwardVector * MaxRange);

		// 카메라 방향 획득
		APlayerController* PC = nullptr;
		if (APawn* Pawn = Cast<APawn>(InSourceActor))
		{
			PC = Cast<APlayerController>(Pawn->GetController());
		}
		if (PC)
		{
			FVector ViewStart;
			FRotator ViewRot;
			PC->GetPlayerViewPoint(ViewStart, ViewRot);

			FVector ViewDir = ViewRot.Vector();
			TraceEnd = ViewStart + (ViewDir * MaxRange);
			
			// 시선 시작점을 카메라 위치로 일치시킴
			TraceStart = ViewStart;
		}

		bool bLocalTraceComplex = false;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(InSourceActor);

		// 시전자에 부착된 모든 액터(화살, 부착형 무기 등) 충돌 무시 대상에 추가
		if (InSourceActor)
		{
			TArray<AActor*> AttachedActors;
			InSourceActor->GetAttachedActors(AttachedActors, true, true);
			ActorsToIgnore.Append(AttachedActors);
		}

		FCollisionQueryParams Params(SCENE_QUERY_STAT(AGATA_ArrowLineTrace), bLocalTraceComplex);
		Params.AddIgnoredActors(ActorsToIgnore);

		FHitResult ReturnHitResult;
		LineTraceWithFilter(ReturnHitResult, GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);
		return ReturnHitResult;
	}
	else
	{
		// 비조준 상태일 때는 캐릭터가 바라보는 전방 방향(Actor Forward Vector)으로 트레이스
		bool bLocalTraceComplex = false;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(InSourceActor);

		// 시전자에 부착된 모든 액터(화살, 부착형 무기 등) 충돌 무시 대상에 추가
		if (InSourceActor)
		{
			TArray<AActor*> AttachedActors;
			InSourceActor->GetAttachedActors(AttachedActors, true, true);
			ActorsToIgnore.Append(AttachedActors);
		}

		FCollisionQueryParams Params(SCENE_QUERY_STAT(AGATA_ArrowLineTrace), bLocalTraceComplex);
		Params.AddIgnoredActors(ActorsToIgnore);

		FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
		if (TraceStart.IsNearlyZero() && InSourceActor)
		{
			TraceStart = InSourceActor->GetActorLocation();
		}
		
		FVector TraceEnd = TraceStart + (InSourceActor->GetActorForwardVector() * MaxRange);

		FHitResult ReturnHitResult;
		LineTraceWithFilter(ReturnHitResult, GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);
		return ReturnHitResult;
	}
}
