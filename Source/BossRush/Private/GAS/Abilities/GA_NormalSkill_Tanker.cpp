// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_NormalSkill_Tanker.h"

UGA_NormalSkill_Tanker::UGA_NormalSkill_Tanker()
{
	SkillRowName = TEXT("NormalSkill_Tanker");
}

void UGA_NormalSkill_Tanker::ExecuteSkillLogic()
{
	Super::ExecuteSkillLogic();
	
	// TODO: Tanker만의 특별한 스킬 로직 추가 (예: 방어 태세, 도발 등)
}
