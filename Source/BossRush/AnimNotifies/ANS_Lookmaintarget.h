// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_Lookmaintarget.generated.h"

/**
 * 애니메이션 재생 도중 보스가 자신의 메인 타겟(CurrentTarget)을 계속 바라보게 회전시키는 AnimNotifyState
 */
UCLASS()
class BOSSRUSH_API UANS_Lookmaintarget : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
