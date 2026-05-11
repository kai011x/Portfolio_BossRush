#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_RopeThrow.generated.h"

/**
 * 로프를 실제로 발사하는 시점에 호출되는 노티파이입니다.
 */
UCLASS()
class BOSSRUSH_API UAN_RopeThrow : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
