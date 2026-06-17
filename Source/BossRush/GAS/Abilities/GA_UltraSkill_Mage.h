// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_UltraSkill.h"
#include "GA_UltraSkill_Mage.generated.h"

/**
 * Mage 전용 궁극기
 */
UCLASS()
class BOSSRUSH_API UGA_UltraSkill_Mage : public UGA_UltraSkill
{
	GENERATED_BODY()

public:
	UGA_UltraSkill_Mage();

protected:
	virtual void ExecuteSkillLogic() override;
};
