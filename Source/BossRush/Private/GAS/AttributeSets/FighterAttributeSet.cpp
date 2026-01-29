// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSets/FighterAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UFighterAttributeSet::UFighterAttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	Resource = 0.0f;
	MaxResource = 100.f;

	RunSpeed = 600.f;
	SprintSpeed = 900.f;
	
	DashCount = 3;
	MaxDashCount = 3;

	Defense = 12.0f;
	Attack = 60.0f;
}

void UFighterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// REPNOTIFY_Always: 예측 실패 시 롤백 등을 위해 항상 알림
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, Resource, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, MaxResource, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, DashCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, MaxDashCount, COND_None, REPNOTIFY_Always);

	// Speed, Attack 등도 UI/로직 동기화를 위해 REPNOTIFY_Always 추천
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, RunSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFighterAttributeSet, SprintSpeed, COND_None, REPNOTIFY_Always);
}

void UFighterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// GE 적용 후 최종 값 검증 및 Clamp
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		// (옵션) 사망 처리 로직을 여기에 추가 가능
		// if (GetHealth() <= 0.0f) { ... }
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetResourceAttribute())
	{
		SetResource(FMath::Clamp(GetResource(), 0.0f, GetMaxResource()));
	}
	else if (Data.EvaluatedData.Attribute == GetDashCountAttribute())
	{
		SetDashCount(FMath::Clamp(GetDashCount(), 0.0f, GetMaxDashCount()));
	}
}



void UFighterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 값이 변경되기 전에 Max 값을 넘지 않도록 Clamp
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetResourceAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxResource());
	}
	else if (Attribute == GetDashCountAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxDashCount());
	}
}




