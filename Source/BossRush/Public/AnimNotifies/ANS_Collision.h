#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ActionDatas.h"
#include "ANS_Collision.generated.h"

/**
 * 애니메이션 도중 공격 판정 콜리전을 켜고 끄며, 피격 타입을 지정하는 NotifyState입니다.
 */
UCLASS()
class BOSSRUSH_API UANS_Collision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EHitType HitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int32 HitIdx = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float LaunchDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float LaunchHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DamageMultiplier = 1.0f;
};
