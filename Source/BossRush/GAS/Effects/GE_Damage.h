#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_Damage.generated.h"

/**
 * C++로 정의된 데미지 적용용 Gameplay Effect 클래스입니다.
 */
UCLASS()
class BOSSRUSH_API UGE_Damage : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_Damage();
};
