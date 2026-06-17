#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ActionDatas.h"
#include "ANS_RadialDamage.generated.h"

/**
 * 애니메이션 구간 동안 즉발 또는 지속적인 범위 데미지를 입히는 NotifyState입니다.
 */
UCLASS()
class BOSSRUSH_API UANS_RadialDamage : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage")
	ERadialDamageType DamageType = ERadialDamageType::Instant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage")
	float Radius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage")
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage")
	EHitType HitType = EHitType::Light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage")
	int32 HitIdx = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage")
	float LaunchDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage")
	float LaunchHeight = 0.0f;

	// 다단히트형일 때 데미지가 들어가는 간격 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radial Damage", meta = (EditCondition = "DamageType == ERadialDamageType::MultiHit"))
	float Interval = 0.2f;

private:
	float LastHitTime = 0.0f;
};
