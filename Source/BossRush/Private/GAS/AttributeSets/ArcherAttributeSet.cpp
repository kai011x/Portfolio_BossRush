// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AttributeSets/ArcherAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UArcherAttributeSet::UArcherAttributeSet()
{
	Health = 90.f;
	MaxHealth = 90.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	Resource = 0.0f;
	MaxResource = 100.f;

	RunSpeed = 700.f;
	SprintSpeed = 950.f;
	AttackLevel = 1.f;


	DashCount = 1;
	MaxDashCount = 1;

	Defense = 12.0f;
	Attack = 50.0f;
	Critical = 0.15f; // 15% 크리티컬 확률

	DashStrength = 1500.0f;
}

void UArcherAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UArcherAttributeSet, AttackLevel, COND_None, REPNOTIFY_Always);
}
