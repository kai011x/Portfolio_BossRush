// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_NormalSkill.h"
#include "GA_NormalSkill_Fighter.generated.h"

/**
 * Fighter 전용 일반 스킬 (공중 제비 및 착지 처리)
 */
UCLASS()
class BOSSRUSH_API UGA_NormalSkill_Fighter : public UGA_NormalSkill
{
	GENERATED_BODY()

public:
	UGA_NormalSkill_Fighter();

protected:
	virtual void ExecuteSkillLogic() override;

	UFUNCTION()
	void OnLanded(EMovementMode NewMode);
};
