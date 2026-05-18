// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AttributeSets/BasicAttributeSet.h"
#include "BossAttributeSet.generated.h"

/**
 * 보스 캐릭터 전용 어트리뷰트 세트
 */
UCLASS()
class BOSSRUSH_API UBossAttributeSet : public UBasicAttributeSet
{
	GENERATED_BODY()

public:
	UBossAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/* --- 보스 전용 어트리뷰트 --- */

	// 보스 쉴드 (기믹용)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UBossAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxShield)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UBossAttributeSet, MaxShield);

	// 보스 그로기 (Basic에서 보스 전용으로 이동)
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxGroggy)
	FGameplayAttributeData MaxGroggy;
	ATTRIBUTE_ACCESSORS(UBossAttributeSet, MaxGroggy);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Groggy)
	FGameplayAttributeData Groggy;
	ATTRIBUTE_ACCESSORS(UBossAttributeSet, Groggy);

	// 보스 현재 페이즈
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Phase)
	FGameplayAttributeData Phase;
	ATTRIBUTE_ACCESSORS(UBossAttributeSet, Phase);

public:
	UFUNCTION()
	void OnRep_Shield(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBossAttributeSet, Shield, OldValue); }

	UFUNCTION()
	void OnRep_MaxShield(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBossAttributeSet, MaxShield, OldValue); }

	UFUNCTION()
	void OnRep_MaxGroggy(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBossAttributeSet, MaxGroggy, OldValue); }

	UFUNCTION()
	void OnRep_Groggy(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBossAttributeSet, Groggy, OldValue); }

	UFUNCTION()
	void OnRep_Phase(const FGameplayAttributeData& OldValue) const { GAMEPLAYATTRIBUTE_REPNOTIFY(UBossAttributeSet, Phase, OldValue); }
};
