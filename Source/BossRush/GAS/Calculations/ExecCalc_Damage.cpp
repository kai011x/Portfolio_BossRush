#include "GAS/Calculations/ExecCalc_Damage.h"
#include "GAS/Tags/GameplayTags.h"
#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "AbilitySystemComponent.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Critical);

	FDamageStatics()
	{
		// 공격자의 공격력과 치명타 확률을 캡처
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, Critical, Source, false);

		// 방어자의 방어력을 캡처
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBasicAttributeSet, Defense, Target, false);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AttackValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluationParameters, AttackValue);

	float DefenseValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParameters, DefenseValue);

	float CriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalDef, EvaluationParameters, CriticalChance);

	// SetByCaller로 넘어온 공격 배율 읽기 (없으면 기본값 1.0)
	float DamageMultiplier = Spec.GetSetByCallerMagnitude(FGameplayTags::Get().DamageMultiplierDataTag, false, 1.0f);

	// 최종 데미지 계산: (공격력 * 배율) - 방어력 (최소 1)
	float Damage = FMath::Max<float>((AttackValue * DamageMultiplier) - DefenseValue, 1.0f);

	// 치명타 판정 (공격자의 Critical 수치보다 작으면 발생)
	float CriticalMultiplier = 1.0f;
	if (FMath::FRandRange(0.0f, 100.0f) <= CriticalChance)
	{
		CriticalMultiplier = 2.0f;
		OutExecutionOutput.AddTargetGameplayTag(FGameplayTag::RequestGameplayTag(FName("Data.Damage.Critical")));
	}

	float FinalDamage = Damage * CriticalMultiplier;

	if (FinalDamage > 0.f)
	{
		// 최종 계산된 데미지를 타겟의 Damage Meta Attribute에 더함
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UBasicAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, FinalDamage));
	}
}
