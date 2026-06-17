// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AttributeSets/TankerAttributeSet.h"

UTankerAttributeSet::UTankerAttributeSet()
{
	// 기본값 설정 (필요 시 BasicAttributeSet의 기본값을 덮어씁니다)
	Health = 150.f;
	MaxHealth = 150.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
}
