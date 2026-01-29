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

public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Dash")
	FVector GetDashDirection(AActor* SourceActor);

	// 블루프린트에서 대시 시작할 때 호출할 함수
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void ResetRegenTimer();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<class UGameplayEffect> DashRegenEffectClass;
};
