#include "GAS/Effects/GE_Damage.h"
#include "GAS/Calculations/ExecCalc_Damage.h"

UGE_Damage::UGE_Damage()
{
	// 1. 즉발형(Instant) 데미지로 설정
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// 2. 데미지 계산 클래스(ExecCalc_Damage) 등록
	FGameplayEffectExecutionDefinition Exec;
	Exec.CalculationClass = UExecCalc_Damage::StaticClass();
	
	Executions.Add(Exec);
}
