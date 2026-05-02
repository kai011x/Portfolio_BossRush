// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AttributeSets/MageAttributeSet.h"

UMageAttributeSet::UMageAttributeSet()
{
	// 기본값 설정 (필요 시 BasicAttributeSet의 기본값을 덮어씁니다)
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
}
