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

	DashStrength = 2000.0f;
}



