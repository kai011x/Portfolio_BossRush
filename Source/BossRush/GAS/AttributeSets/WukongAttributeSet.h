// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AttributeSets/BossAttributeSet.h"
#include "WukongAttributeSet.generated.h"

/**
 * 우콩(Wukong) 보스 전용 어트리뷰트 세트
 */
UCLASS()
class BOSSRUSH_API UWukongAttributeSet : public UBossAttributeSet
{
	GENERATED_BODY()

public:
	UWukongAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/* --- 우콩 전용 어트리뷰트 --- */

	// 요력 (패턴 강화용 리소스)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MonkeyPower)
	FGameplayAttributeData MonkeyPower;
	ATTRIBUTE_ACCESSORS(UWukongAttributeSet, MonkeyPower);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxMonkeyPower)
	FGameplayAttributeData MaxMonkeyPower;
	ATTRIBUTE_ACCESSORS(UWukongAttributeSet, MaxMonkeyPower);

	// 여의봉 스택 (공격 시 중첩되어 데미지나 크기 증가)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_StaffStack)
	FGameplayAttributeData StaffStack;
	ATTRIBUTE_ACCESSORS(UWukongAttributeSet, StaffStack);

public:
	UFUNCTION()
	void OnRep_MonkeyPower(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UWukongAttributeSet, MonkeyPower, OldValue); }

	UFUNCTION()
	void OnRep_MaxMonkeyPower(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UWukongAttributeSet, MaxMonkeyPower, OldValue); }

	UFUNCTION()
	void OnRep_StaffStack(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UWukongAttributeSet, StaffStack, OldValue); }
};
