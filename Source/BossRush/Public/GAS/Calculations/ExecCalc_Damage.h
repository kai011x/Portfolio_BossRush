#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 공격자의 Attack, Critical 스탯과 방어자의 Defense 스탯을 사용하여
 * 최종 데미지를 계산하고 치명타 판정을 수행하는 클래스입니다.
 */
UCLASS()
class BOSSRUSH_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
