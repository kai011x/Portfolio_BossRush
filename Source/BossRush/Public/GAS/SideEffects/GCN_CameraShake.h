// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GCN_CameraShake.generated.h"

class UCameraShakeBase;

/**
 * 카메라 흔들림을 발생시키는 범용 GameplayCue 클래스입니다.
 * 네트워크 환경에서 복제되어 (Listen Server / Client) 범위 내에 있는 플레이어들에게 화면 흔들림을 적용합니다.
 */
UCLASS()
class BOSSRUSH_API UGCN_CameraShake : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	UGCN_CameraShake();

	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	float InnerRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	float OuterRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraShake")
	float Falloff;
};
