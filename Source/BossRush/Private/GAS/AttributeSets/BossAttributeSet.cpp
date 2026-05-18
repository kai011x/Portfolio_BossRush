// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSets/BossAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UBossAttributeSet::UBossAttributeSet()
{
	Shield = 0.0f;
	MaxShield = 0.0f;
	Groggy = 0.0f;
	MaxGroggy = 100.0f;
	Phase = 1.0f;
}

void UBossAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBossAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBossAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBossAttributeSet, Groggy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBossAttributeSet, MaxGroggy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBossAttributeSet, Phase, COND_None, REPNOTIFY_Always);
}

void UBossAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// 부모 클래스의 대미지 처리 로직 등을 먼저 수행
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
	}
	else if (Data.EvaluatedData.Attribute == GetGroggyAttribute())
	{
		SetGroggy(FMath::Clamp(GetGroggy(), 0.0f, GetMaxGroggy()));
		
		if (GetGroggy() >= GetMaxGroggy())
		{
			UE_LOG(LogTemp, Warning, TEXT("[BossAttributeSet] Boss is GROGGY!"));
		}
	}
}
