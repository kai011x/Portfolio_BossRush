// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_NormalSkill_Mage.h"

UGA_NormalSkill_Mage::UGA_NormalSkill_Mage()
{
	SkillRowName = TEXT("NormalSkill_Mage");
}

void UGA_NormalSkill_Mage::ExecuteSkillLogic()
{
	Super::ExecuteSkillLogic();
	
	// TODO: Mage만의 특별한 스킬 로직 추가 (예: 마법진 생성, 힐, 버프 등)
}
