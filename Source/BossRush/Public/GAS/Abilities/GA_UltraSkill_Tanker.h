// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_UltraSkill.h"
#include "GA_UltraSkill_Tanker.generated.h"

/**
 * Tanker 전용 궁극기
 */
UCLASS()
class BOSSRUSH_API UGA_UltraSkill_Tanker : public UGA_UltraSkill
{
	GENERATED_BODY()

public:
	UGA_UltraSkill_Tanker();

protected:
	virtual void ExecuteSkillLogic() override;
};
