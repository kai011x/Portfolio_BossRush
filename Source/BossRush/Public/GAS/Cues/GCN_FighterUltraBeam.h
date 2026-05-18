// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "GCN_FighterUltraBeam.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

/**
 * Fighter 궁극기 빔 이펙트를 담당하는 GameplayCue Actor
 */
UCLASS()
class BOSSRUSH_API AGCN_FighterUltraBeam : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	AGCN_FighterUltraBeam();

protected:
	virtual bool OnActive_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
	virtual bool OnRemove_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* BeamSystem;

	UPROPERTY()
	UNiagaraComponent* ActiveBeamComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FName MuzzleSocketName = TEXT("Muzzle_Front");
};
