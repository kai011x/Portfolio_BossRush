// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_NormalSkill.h"
#include "GA_NormalSkill_Archer.generated.h"

/**
 * Archer 전용 일반 스킬
 */
UCLASS()
class BOSSRUSH_API UGA_NormalSkill_Archer : public UGA_NormalSkill
{
	GENERATED_BODY()

public:
	UGA_NormalSkill_Archer();

protected:
	virtual void ExecuteSkillLogic() override;
};
