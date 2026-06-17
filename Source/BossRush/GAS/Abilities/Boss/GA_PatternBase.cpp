// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Boss/GA_PatternBase.h"

UGA_PatternBase::UGA_PatternBase()
{
	// 패턴은 기본적으로 인스턴스화 정책을 아바타마다 하나씩 갖도록 설정 (보통 보스용)
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}
