// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSets/WukongAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UWukongAttributeSet::UWukongAttributeSet()
{
	MonkeyPower = 0.0f;
	MaxMonkeyPower = 100.0f;
	StaffStack = 0.0f;
}

void UWukongAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UWukongAttributeSet, MonkeyPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWukongAttributeSet, MaxMonkeyPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWukongAttributeSet, StaffStack, COND_None, REPNOTIFY_Always);
}

void UWukongAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetMonkeyPowerAttribute())
	{
		SetMonkeyPower(FMath::Clamp(GetMonkeyPower(), 0.0f, GetMaxMonkeyPower()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaffStackAttribute())
	{
		SetStaffStack(FMath::Max(GetStaffStack(), 0.0f));
	}
}
