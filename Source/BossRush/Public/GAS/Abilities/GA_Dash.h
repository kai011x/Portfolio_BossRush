// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API UGA_Dash : public UGameplayAbility
{
	GENERATED_BODY()
	
	UGA_Dash();

	UFUNCTION(BlueprintCallable, Category = "Ability|Dash")
	FVector GetDashDirection(AActor* SourceActor);
};
