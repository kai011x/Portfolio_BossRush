// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/SideEffects/GCN_CameraShake.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "GAS/Tags/GameplayTags.h"

UGCN_CameraShake::UGCN_CameraShake()
{
	// 이 클래스가 처리할 태그를 지정합니다.
	GameplayCueTag = FGameplayTags::Get().GameplaycueCameraShakeTag;
	
	InnerRadius = 0.0f;
	OuterRadius = 1500.0f;
	Falloff = 1.0f;
}

bool UGCN_CameraShake::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	if (CameraShakeClass)
	{
		FVector Location = Parameters.Location;
		
		// 파라미터에 위치가 없다면 타겟의 위치를 사용합니다.
		if (Location.IsZero() && MyTarget)
		{
			Location = MyTarget->GetActorLocation();
		}

		// PlayWorldCameraShake를 통해 범위 내 클라이언트의 카메라를 흔듭니다.
		UGameplayStatics::PlayWorldCameraShake(
			MyTarget, // WorldContextObject
			CameraShakeClass,
			Location,
			InnerRadius,
			OuterRadius,
			Falloff
		);
	}

	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
